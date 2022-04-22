/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/

#include "criticante.h"

#include <stdbool.h>
#include <stdlib.h>
#include <likwid.h>
// #include <matheval.h>
#include "Rosenbrock.h"

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

/**
 * @brief Itera um Criticante do tipo padrão.
 * 
 * @param c Criticante a ser iterado.
 * @return IterInfo Informações sobre a iteração realizada.
 */
static IterInfo iterar_padrao(Criticante *c);

/**
 * @brief Itera um Criticante do tipo modificado.
 * 
 * @param c Criticante a ser iterado.
 * @return IterInfo Informações sobre a iteração realizada.
 */
static IterInfo iterar_modificado(Criticante *c);

/**
 * @brief Itera um Criticante do tipo inexato.
 * 
 * @param c Criticante a ser iterado.
 * @return IterInfo Informações sobre a iteração realizada.
 */
static IterInfo iterar_inexato(Criticante *c);

/**
 * @brief Cria o vetor de evaluators para o gradiente.
 * 
 * @param c Criticante para qual será criado o vetor de evaluators.
 * @return int 0 caso sucesso, -1 em caso de falha.
 */
static int criar_evaluator_gradiente(Criticante *c);

/**
 * @brief Cria a matriz de evaluators para o hessiana.
 * 
 * @param c Criticante para qual será criado a matriz de evaluators.
 * @return int 0 caso sucesso, -1 em caso de falha.
 */
static int criar_evaluator_hessiana(Criticante *c);

/**
 * @brief Evalua o gradiente no chute atual.
 * 
 * @param c Criticante a ter seu gradiente evaluado.
 */
static void evaluar_gradiente(Criticante *c);

/**
 * @brief Evalua a hessiana no chute atual.
 * 
 * @param c Criticante a ter sua hessiana evaluada.
 */
static void evaluar_hessiana(Criticante *c);

/**
 * @brief Libera a memória usada pela hessiana.
 * 
 * @param c Criticante a ter sua hessiana destruída.
 */
static void destruir_evaluator_hessiana(Criticante *c);

/**
 * @brief Libera a memória usada pela hessiana.
 * 
 * @param c Criticante a ter sua hessiana destruída.
 */
static void destruir_evaluator_gradiente(Criticante *c);

/* 
static int criar_evaluator_hessiana(Criticante *c)
{
    c->hessiana = (void ***)criar_matriz(sizeof(void *), c->num_vars);
    if(!c->hessiana) return -1;

    // Calculando jacobiana do gradiente de f, que é a hessiana de f.
    for (int i = 0; i < c->num_vars; i++)
    {
        for (int j = 0; j < c->num_vars; j++)
        {
            c->hessiana[i][j] = evaluator_derivative(c->gradiente[i], c->f_vars[j]);
            if(!c->hessiana[i][j]) return -1;
        }
    }

    return 0;
}

static void destruir_evaluator_hessiana(Criticante *c) {

    for (int i = 0; i < c->num_vars; i++)
        for (int j = 0; j < c->num_vars; j++)
            if(!c->hessiana[i][j]) evaluator_destroy(c->hessiana[i][j]);
    
    destruir_matriz((void **)c->hessiana, c->num_vars);
}

static int criar_evaluator_gradiente(Criticante *c)
{
    c->gradiente = criar_vetor(sizeof(void *), c->num_vars);
    if(!c->gradiente) return -1;

    // Vetor de derivadas parciais de f
    for (int i = 0; i < c->num_vars; i++) {
        c->gradiente[i] = evaluator_derivative(c->f_evaluator, c->f_vars[i]);
        if(!c->gradiente[i]) return -1;
    }

    return 0;
}

static void destruir_evaluator_gradiente(Criticante *c) {
    for (int i = 0; i < c->num_vars; i++)
        if(!c->gradiente[i]) evaluator_destroy(c->gradiente[i]);

    destruir_vetor(c->gradiente);
}
*/
Criticante *criar_criticante(int n, char* f_str, int max_iters, double epsilon, double *X0, TipoCriticante tipo) {
    Criticante *c = malloc(sizeof(Criticante));
    if (!c) return NULL;
    
    c->info.iteracao = 0;
    c->info.tempo_total = 0;
    c->info.tempo_derivadas = 0;
    c->info.tempo_SL = 0;
    c->info.f_x = 0;
    c->info.ocorreu_erro = false;

    c->info.acabou = false;
    
    c->f_str = f_str;
    // c->f_evaluator = evaluator_create(f_str);
    c->num_vars = n;
    // evaluator_get_variables(c->f_evaluator, &c->f_vars, &c->num_vars);
    
    // if (criar_evaluator_gradiente(c) == -1) {
    //     free(c);
    //     return NULL;
    // }

    // if (criar_evaluator_hessiana(c) == -1) {
    //     destruir_evaluator_gradiente(c);
    //     free(c);
    //     return NULL;
    // }
    c->gradiente_evaluado = (double *) criar_vetor(sizeof(double), c->num_vars);
    if (!c->gradiente_evaluado) {
        // destruir_evaluator_hessiana(c);
        // destruir_evaluator_gradiente(c);
        free(c);
        return NULL;
    }
    c->hessiana_evaluada = (double **) criar_matriz(sizeof(double), c->num_vars);
    if (!c->hessiana_evaluada) {
        free(c->gradiente_evaluado);
        // destruir_evaluator_hessiana(c);
        // destruir_evaluator_gradiente(c);
        free(c);
        return NULL;
    }

    c->X = (double *) criar_vetor(sizeof(double), c->num_vars);
    if (!c->hessiana_evaluada) {
        free(c->hessiana_evaluada);
        free(c->gradiente_evaluado);
        // destruir_evaluator_hessiana(c);
        // destruir_evaluator_gradiente(c);
        free(c);
        return NULL;
    }

    copiar_vetor_double(c->X, X0, c->num_vars);
    
    c->max_iters = max_iters;
    c->epsilon = epsilon;
    
    c->tipo = tipo;
    c->newton = alocar_sl(c->num_vars);
    if (!c->newton) {
        free(c->hessiana_evaluada);
        free(c->gradiente_evaluado);
        // destruir_evaluator_hessiana(c);
        // destruir_evaluator_gradiente(c);
        free(c);
        return NULL;
    }
    
    switch (tipo) {
        case CRITICANTE_INEXATO:
            c->config.gauss_seidel = alocar_config_gauss_seidel(c->newton);
            if (!c->config.gauss_seidel) {
                destruir_sl(c->newton);
                free(c->hessiana_evaluada);
                free(c->gradiente_evaluado);
                // destruir_evaluator_hessiana(c);
                // destruir_evaluator_gradiente(c);
                free(c);
                return NULL;
            }
            break;

        case CRITICANTE_MODIFICADO:
            c->config.LU = alocar_config_LU(c->newton);
            if (!c->config.LU) {
                destruir_sl(c->newton);
                free(c->hessiana_evaluada);
                free(c->gradiente_evaluado);
                // destruir_evaluator_hessiana(c);
                // destruir_evaluator_gradiente(c);
                free(c);
                return NULL;
            }
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

    // c->info.f_x = evaluator_evaluate(c->f_evaluator, c->num_vars, c->f_vars, c->X);
    c->info.f_x = rosenbrock(c->X, c->num_vars);
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
    
    // evaluator_destroy(crit->f_evaluator);

    // destruir_evaluator_gradiente(crit);
    destruir_vetor(crit->gradiente_evaluado);

    // destruir_evaluator_hessiana(crit);
    destruir_matriz((void **)crit->hessiana_evaluada, crit->num_vars);
    
    destruir_vetor(crit->X);
    
    destruir_sl(crit->newton);
    destruir_config(crit);
    
    free(crit);
}

static void evaluar_gradiente(Criticante *c) {
    double tempo = timestamp();

    
    /* 
    for (int i = 0; i < c->num_vars; i++)
        c->gradiente_evaluado[i] = -evaluator_evaluate(c->gradiente[i], c->num_vars, c->f_vars, c->X);
     */
    for (int i = 0; i < c->num_vars; i++)
        c->gradiente_evaluado[i] = -rosenbrock_dx(i, c->X, c->num_vars);

    tempo = timestamp() - tempo;
    c->info.tempo_derivadas += tempo;
}

static void evaluar_hessiana(Criticante *c) {
    double tempo = timestamp();

    /* for (int i = 0; i < c->num_vars; i++)
    {
        for (int j = 0; j < c->num_vars; j++)
        {
            c->hessiana_evaluada[i][j] = evaluator_evaluate(c->hessiana[i][j], c->num_vars, c->f_vars, c->X);
        }
    }
    */
    for (int i = 0; i < c->num_vars; i++)
    {
        for (int j = 0; j < c->num_vars; j++)
        {
            c->hessiana_evaluada[i][j] = rosenbrock_dxdy(i, j, c->X, c->num_vars);
        }
    }

    tempo = timestamp() - tempo;
    c->info.tempo_derivadas += tempo;
}

static IterInfo iterar_padrao(Criticante *c) {
    LIKWID_MARKER_START("newton-padrao-total");

    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();

    LIKWID_MARKER_START("newton-padrao-gradiente");
    evaluar_gradiente(c);
    LIKWID_MARKER_STOP("newton-padrao-gradiente");

    if (norma(c->gradiente_evaluado, c->num_vars) < c->epsilon) {
        c->info.acabou = true;

        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;
        
        LIKWID_MARKER_STOP("newton-padrao-total");
        return c->info;
    }

    LIKWID_MARKER_START("newton-padrao-hessiana");
    evaluar_hessiana(c);
    LIKWID_MARKER_STOP("newton-padrao-hessiana");

    LIKWID_MARKER_START("newton-padrao-sl");
    tempo_SL = timestamp();

    criar_sl(c->newton, c->hessiana_evaluada, c->gradiente_evaluado);
    resolver_sl_eliminacao_gauss(c->newton);

    tempo_SL = timestamp() - tempo_SL;
    c->info.tempo_SL += tempo_SL;
    LIKWID_MARKER_STOP("newton-padrao-sl");
    
    double *delta = solucao_sl(c->newton);
    if(!delta) {
        c->info.ocorreu_erro = true;
        c->info.acabou = true;
        return c->info;
    }

    somar_vetor(c->X, delta, c->num_vars);
    
    if (norma(delta, c->num_vars) < c->epsilon)
    {
        c->info.acabou = true;

        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;
        
        LIKWID_MARKER_STOP("newton-padrao-total");
        return c->info;
    }

    tempo_total = timestamp() - tempo_total;
    c->info.tempo_total += tempo_total;

    LIKWID_MARKER_STOP("newton-padrao-total");
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
        setar_matriz_sl(c->newton, c->hessiana_evaluada);
        criar_config_LU(c->config.LU, c->newton);
    }
    setar_termos_independentes_sl(c->newton, c->gradiente_evaluado);

    tempo_SL = timestamp();

    resolver_sl_LU(c->newton, c->config.LU);

    tempo_SL = timestamp() - tempo_SL;
    c->info.tempo_SL += tempo_SL;

    double *delta = solucao_sl(c->newton);
    if(!delta) {
        c->info.ocorreu_erro = true;
        c->info.acabou = true;
        return c->info;
    }

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
    LIKWID_MARKER_START("newton-inexato-total");

    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();
    
    LIKWID_MARKER_START("newton-inexato-gradiente");
    evaluar_gradiente(c);
    LIKWID_MARKER_STOP("newton-inexato-gradiente");

    if (norma(c->gradiente_evaluado, c->num_vars) < c->epsilon) {
        c->info.acabou = true;
        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;

        LIKWID_MARKER_STOP("newton-inexato-total");
        return c->info;
    }

    LIKWID_MARKER_START("newton-inexato-hessiana");
    evaluar_hessiana(c);
    LIKWID_MARKER_STOP("newton-inexato-hessiana");
    
    LIKWID_MARKER_START("newton-inexato-sl");
    tempo_SL = timestamp();

    criar_sl(c->newton, c->hessiana_evaluada, c->gradiente_evaluado);
    criar_config_gauss_seidel(c->config.gauss_seidel, c->newton);
    resolver_sl_gauss_seidel(c->newton, c->config.gauss_seidel);

    tempo_SL = timestamp() - tempo_SL;
    c->info.tempo_SL += tempo_SL;
    LIKWID_MARKER_STOP("newton-inexato-sl");

    double *delta = solucao_sl(c->newton);
    if(!delta) {
        c->info.ocorreu_erro = true;
        c->info.acabou = true;
        return c->info;
    }

    somar_vetor(c->X, delta, c->num_vars);
    
    if (norma(delta, c->num_vars) < c->epsilon) {
        c->info.acabou = true;
        tempo_total = timestamp() - tempo_total;
        c->info.tempo_total += tempo_total;

        LIKWID_MARKER_STOP("newton-inexato-total");
        return c->info;
    }

    tempo_total = timestamp() - tempo_total;
    c->info.tempo_total += tempo_total;
    
    LIKWID_MARKER_STOP("newton-inexato-total");
    return c->info;
}

double *solucao_criticante(Criticante* c) {
    return c->X;
}
