#include "criticante.h"

#include <stdbool.h>
#include <stdlib.h>
#include <matheval.h>

#include "utils.h"

#include "sistema.h"
#include "utilidades.h"

typedef struct Criticante {
    IterInfo info;
    
    char * f_str;
    void * f_evaluator;
    char ** f_vars;
    int num_vars;

    void *** hessiana;
    void ** gradiente;
    double ** hessiana_evaluada;
    double * gradiente_evaluado;
    double * X;

    int max_iters;
    double epsilon;
    
    TipoCriticante tipo;

    SistemaLinear *newton;
    
    union {
        ConfigLU *LU;
        ConfigGaussSeidel *gauss_seidel;
    } config;
    
} Criticante;

static IterInfo iterar_padrao(Criticante *c);
static IterInfo iterar_modificado(Criticante *c);
static IterInfo iterar_inexato(Criticante *c);

static void criar_evaluator_gradiente(Criticante *c);
static void criar_evaluator_hessiana(Criticante *c);

static void evaluar_gradiente(Criticante *c);
static void evaluar_hessiana(Criticante *c);


static void criar_evaluator_hessiana(Criticante *c)
{
    c->hessiana = (void ***)criar_matriz(sizeof(void *), c->num_vars);

    // Calculando jacobiana do gradiente de f, que é a hessiana de f.
    for (int i = 0; i < c->num_vars; i++)
    {
        for (int j = 0; j < c->num_vars; j++)
        {
            c->hessiana[i][j] = evaluator_derivative(c->gradiente[i], c->f_vars[j]);
        }
    }
}

static void criar_evaluator_gradiente(Criticante *c)
{
    c->gradiente = criar_vetor(sizeof(void *), c->num_vars);
    // evaluator_get_variables(c->f_evaluator, &vars, &count);

    // Vetor de derivadas parciais de f
    for (int i = 0; i < c->num_vars; i++)
        c->gradiente[i] = evaluator_derivative(c->f_evaluator, c->f_vars[i]);
}

Criticante *criar_criticante(char* f_str, int max_iters, double epsilon, double *X0, TipoCriticante tipo) {
    Criticante *c = malloc(sizeof(Criticante));
    
    c->info.iteracao = 0;
    c->info.tempo_total = 0;
    c->info.tempo_derivadas = 0;
    c->info.tempo_SL = 0;
    c->info.f_x = 0;

    c->info.acabou = false;
    
    c->f_str = f_str;
    c->f_evaluator = evaluator_create(f_str);
    
    evaluator_get_variables(c->f_evaluator, &c->f_vars, &c->num_vars);
    
    criar_evaluator_gradiente(c);
    criar_evaluator_hessiana(c);
    c->gradiente_evaluado = (double *) criar_vetor(sizeof(double), c->num_vars);
    c->hessiana_evaluada = (double **) criar_matriz(sizeof(double), c->num_vars);
    c->X = (double *) criar_vetor(sizeof(double), c->num_vars);
    copiar_vetor_double(c->X, X0, c->num_vars);
    
    c->max_iters = max_iters;
    c->epsilon = epsilon;
    
    c->tipo = tipo;
    c->newton = alocar_sl(c->num_vars);
    
    switch (tipo) {
        case CRITICANTE_INEXATO:
            c->config.gauss_seidel = alocar_config_gauss_seidel(c->newton);
            break;

        case CRITICANTE_MODIFICADO:
            c->config.LU = alocar_config_LU(c->newton);
            break;
    }

    return c;
}


IterInfo iterar_criticante(Criticante *c) {

    // TODO der erro se chegou no maximo de iteracao

    c->info.iteracao++;

    if(c->info.acabou)
        return c->info;

    if(c->info.iteracao > c->max_iters) {
        c->info.acabou = true;
    }

    switch (c->tipo)
    {
    case CRITICANTE_PADRAO:
        iterar_padrao(c);
        break;

    case CRITICANTE_MODIFICADO:
        iterar_modificado(c);
        break;

    case CRITICANTE_INEXATO:
        iterar_inexato(c);
        break;
    
    default:
        break;
    }

    c->info.f_x = evaluator_evaluate(c->f_evaluator, c->num_vars, c->f_vars, c->X);

    return c->info;
}

bool acabou_criticante(Criticante *crit) {
    return crit->info.acabou;
}

void destruir_config(Criticante *crit) {
    switch (crit->tipo) {

    case CRITICANTE_MODIFICADO:
        destruir_config_LU(crit->config.LU);
        break;

    case CRITICANTE_INEXATO:
        destruir_config_gauss_seidel(crit->config.gauss_seidel);
        break;
    
    default:
        break;
    }
}

void destruir_criticante(Criticante *crit) {
    
    evaluator_destroy(crit->f_evaluator);

    for (int i = 0; i < crit->num_vars; i++)
        evaluator_destroy(crit->gradiente[i]);
    
    destruir_vetor(crit->gradiente);
    destruir_vetor(crit->gradiente_evaluado);

    for (int i = 0; i < crit->num_vars; i++)
        for (int j = 0; j < crit->num_vars; j++)
            evaluator_destroy(crit->hessiana[i][j]);

    destruir_matriz((void **)crit->hessiana, crit->num_vars);
    destruir_matriz((void **)crit->hessiana_evaluada, crit->num_vars);
    
    destruir_vetor(crit->X);
    
    destruir_sl(crit->newton);
    destruir_config(crit);
    
    free(crit);
}

static void evaluar_gradiente(Criticante *c) {
    double tempo = timestamp();
    for (int i = 0; i < c->num_vars; i++)
        c->gradiente_evaluado[i] = -evaluator_evaluate(c->gradiente[i], c->num_vars, c->f_vars, c->X);
    
    tempo = timestamp() - tempo;
    c->info.tempo_derivadas += tempo;
}

static void evaluar_hessiana(Criticante *c) {
    double tempo = timestamp();

    for (int i = 0; i < c->num_vars; i++)
    {
        for (int j = 0; j < c->num_vars; j++)
        {
            c->hessiana_evaluada[i][j] = evaluator_evaluate(c->hessiana[i][j], c->num_vars, c->f_vars, c->X);
        }
    }

    tempo = timestamp() - tempo;
    c->info.tempo_derivadas += tempo;
}

static IterInfo iterar_padrao(Criticante *c) {
    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();

    evaluar_gradiente(c);

    if (norma(c->gradiente_evaluado, c->num_vars) < c->epsilon) {
        c->info.acabou = true;

        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;
        
        return c->info;
    }

    evaluar_hessiana(c);

    tempo_SL = timestamp();
    
    criar_sl(c->newton, c->hessiana_evaluada, c->gradiente_evaluado);
    resolver_sl_eliminacao_gauss(c->newton);

    tempo_SL = timestamp() - tempo_SL;
    c->info.tempo_SL += tempo_SL;
    
    double *delta = solucao_sl(c->newton);

    somar_vetor(c->X, delta, c->num_vars);
    
    if (norma(delta, c->num_vars) < c->epsilon)
    {
        c->info.acabou = true;

        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;
        
        return c->info;
    }

    tempo_total = timestamp() - tempo_total;
    c->info.tempo_total += tempo_total;

    return c->info;
}

static IterInfo iterar_modificado(Criticante *c) {
    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    const int hess_steps = c->num_vars;
    
    tempo_total = timestamp();
    
    evaluar_gradiente(c);
    
    if (norma(c->gradiente_evaluado, c->num_vars) < c->epsilon) {
        c->info.acabou = true;

        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;

        return c->info;
    }


    if (!((c->info.iteracao - 1) % hess_steps)) {
        evaluar_hessiana(c);
        criar_sl(c->newton, c->hessiana_evaluada, c->gradiente_evaluado);
        criar_config_LU(c->config.LU, c->newton);
    } else {
        criar_sl(c->newton, c->hessiana_evaluada, c->gradiente_evaluado);
    }

    tempo_SL = timestamp();

    resolver_sl_LU(c->newton, c->config.LU);

    tempo_SL = timestamp() - tempo_SL;
    c->info.tempo_SL += tempo_SL;

    double *delta = solucao_sl(c->newton);

    somar_vetor(c->X, delta, c->num_vars);

    if (norma(delta, c->num_vars) < c->epsilon)
    {
        c->info.acabou = true;

        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;
        
        return c->info;
    }

    tempo_total = timestamp() - tempo_total;
    c->info.tempo_total += tempo_total;

    return c->info;
}

static IterInfo iterar_inexato(Criticante *c) {
    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();
    
    evaluar_gradiente(c);

    if (norma(c->gradiente_evaluado, c->num_vars) < c->epsilon) {
        c->info.acabou = true;
        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;

        return c->info;
    }

    evaluar_hessiana(c);
    
    tempo_SL = timestamp();

    criar_sl(c->newton, c->hessiana_evaluada, c->gradiente_evaluado);
    criar_config_gauss_seidel(c->config.gauss_seidel, c->newton);
    resolver_sl_gauss_seidel(c->newton, c->config.gauss_seidel);

    tempo_SL = timestamp() - tempo_SL;
    c->info.tempo_SL += tempo_SL;

    double *delta = solucao_sl(c->newton);
    somar_vetor(c->X, delta, c->num_vars);
    
    if (norma(delta, c->num_vars) < c->epsilon) {
        c->info.acabou = true;
        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;

        return c->info;
    }
    
    tempo_total = timestamp() - tempo_total;
    c->info.tempo_total += tempo_total;
    
    return c->info;
}

double *solucao_criticante(Criticante* c) {
    return c->X;
}