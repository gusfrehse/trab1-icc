#include "newton.h"
#include <stdlib.h>
#include <stdio.h>

#include <matheval.h>
#include <math.h>

#include "sim.h"
#include "utilidades.h"

static void calcula_gradiente_para_iteracao(iteracao *iter)
{
    iter->gradiente = cria_vetor(sizeof(void *), iter->n);
    // evaluator_get_variables(iter->f_evaluator, &vars, &count);

    // Vetor de derivadas parciais de f
    for (int i = 0; i < iter->n; i++)
        iter->gradiente[i] = evaluator_derivative(iter->f_evaluator, iter->nomes_vars[i]);
}

// necessita ter calculado o gradiente antes
static void calcula_hessiana_para_iteracao(iteracao *iter)
{
    iter->hessiana = (void ***)cria_matriz(sizeof(void *), iter->n);

    // Calculando jacobiana do gradiente de f, que é a hessiana de f.
    for (int i = 0; i < iter->n; i++)
    {
        for (int j = 0; j < iter->n; j++)
        {
            iter->hessiana[i][j] = evaluator_derivative(iter->gradiente[i], iter->nomes_vars[j]);
        }
    }
}
// char* s = "teste";
// char* b = s + 3; // == "te"
//
// int* i = { 1, 2, 3, 4 };
// int* d = (char *) i + 2; // == { 3, 4 }

iteracao *criar_iteracao(char *f_str, int n, double *chute_inicial, double epsilon, double max_iteracoes)
{
    int count;
    iteracao *iter = malloc(sizeof(iteracao));

    iter->i = 0;
    iter->n = n;
    iter->tempo_total = 0;
    iter->tempo_derivadas = 0;
    iter->tempo_SL = 0;
    iter->f_str = f_str;
    iter->f_evaluator = evaluator_create(f_str);

    // TODO: checar se iter-> == count
    evaluator_get_variables(iter->f_evaluator, &(iter->nomes_vars), &count);

    calcula_gradiente_para_iteracao(iter);
    calcula_hessiana_para_iteracao(iter);

    iter->L = (double **)cria_matriz(sizeof(double), n);
    iter->U = (double **)cria_matriz(sizeof(double), n);

    iter->X = chute_inicial;
    iter->hess_steps = n;
    iter->epsilon = epsilon;
    iter->max_iteracoes = max_iteracoes;
    iter->acabou = false;
}

void destroi_iteracao(iteracao *iter)
{
    evaluator_destroy(iter->f_evaluator);

    for (int i = 0; i < iter->n; i++)
    {
        for (int j = 0; j < iter->n; j++)
        {
            evaluator_destroy(iter->hessiana[i][j]);
        }
    }

    destroi_matriz((void **)iter->hessiana, iter->n);

    for (int i = 0; i < iter->n; i++)
    {
        evaluator_destroy(iter->gradiente[i]);
    }

    // TODO: destroi_vetor
    free(iter->gradiente);

    destroi_matriz((void **)iter->L, iter->n);
    destroi_matriz((void **)iter->U, iter->n);

    free(iter->X);

    free(iter);
}

static double norma(double *X, int n)
{
    double max = fabs(X[0]);
    for (int i = 1; i < n; i++)
    {
        if (fabs(X[i]) > max)
            max = fabs(X[i]);
    }

    return max;
}

iteracao *iterar_newton_padrao(iteracao *iter)
{
    double *gradiente_evaluado = (double *)cria_vetor(sizeof(double), iter->n);

    for (int i = 0; i < iter->n; i++)
    {
        gradiente_evaluado[i] = -evaluator_evaluate(iter->gradiente[i],
                                                    iter->n,
                                                    iter->nomes_vars,
                                                    iter->X);
        debug_print("%f ", gradiente_evaluado[i]);
    }

    double norma_evaluada = norma(gradiente_evaluado, iter->n);

    if (norma_evaluada < iter->epsilon)
    {
        free(gradiente_evaluado);
        iter->acabou = true;
        iter->i++;
        return;
    }

    double **hessiana_evaluada = (double **)cria_matriz(sizeof(double), iter->n);

    for (int i = 0; i < iter->n; i++)
    {
        for (int j = 0; j < iter->n; j++)
        {
            hessiana_evaluada[i][j] = evaluator_evaluate(iter->hessiana[i][j],
                                                         iter->n,
                                                         iter->nomes_vars,
                                                         iter->X);
        }
    }

    iter->i++;

    double *delta = resolver_sistema(hessiana_evaluada, gradiente_evaluado, iter->n);

    free(gradiente_evaluado);
    free(hessiana_evaluada);

    for (int i = 0; i < iter->n; i++)
    {
        iter->X[i] += delta[i];
    }

    if (norma(delta, iter->n) < iter->epsilon)
    {
        iter->acabou = true;
        free(delta);
        return;
    }

    free(delta); // B)
}

/* iteracao* iterar_newton_modificado(iteracao* iter) {
    static double **hessiana_evaluada = NULL;
    // static bool ok = false;
    double* gradiente_evaluado = (double*) cria_vetor(sizeof(double), iter->n);

    for (int i = 0; i < iter->n; i++)
    {
        gradiente_evaluado[i] = evaluator_evaluate(iter->gradiente[i],
                                                   iter->n,
                                                   iter->nomes_vars,
                                                   iter->X);
    }

    double norma_evaluada = norma(gradiente_evaluado, iter->n);

    if (norma_evaluada < iter->epsilon) {
        free(gradiente_evaluado);
        iter->acabou = true;
        iter->i++;
        return;
    }

    if(!(iter->i % iter->hess_steps)) {
        if(!hessiana_evaluada) {
            hessiana_evaluada = (double **) cria_matriz(sizeof(double), iter->n);
        }

        for (int i = 0; i < iter->n; i++) {
            for (int j = 0; j < iter->n; j++) {
                hessiana_evaluada[i][j] = evaluator_evaluate(iter->hessiana[i][j],
                                                            iter->n,
                                                            iter->nomes_vars,
                                                            iter->X);
            }
        }
    }

    // TDO: TA NO MEIO ISSO WTF
    iter->i++;

    double *delta = resolver_sistema_LU(hessiana_evaluada, gradiente_evaluado);

    free(gradiente_evaluado);
    // free(hessiana_evaluada);

    for (int i = 0; i < iter->n; i++) {
        iter->X[i] += delta[i];
    }


    if (norma(delta, iter->n) < iter->epsilon) {
        iter->acabou = true;
        free(delta);
        return;
    }

    free(delta); // B)
}

iteracao* iterar_newton_inexato(iteracao* iter) {
    double* gradiente_evaluado = (double*) cria_vetor(sizeof(double), iter->n);

    for (int i = 0; i < iter->n; i++)
    {
        gradiente_evaluado[i] = evaluator_evaluate(iter->gradiente[i],
                                                   iter->n,
                                                   iter->nomes_vars,
                                                   iter->X);
    }

    double norma_evaluada = norma(gradiente_evaluado, iter->n) ;

    if (norma_evaluada < iter->epsilon) {
        free(gradiente_evaluado);
        iter->acabou = true;
        iter->i++;
        return;
    }

    double **hessiana_evaluada = (double **) cria_matriz(sizeof(double), iter->n);

    for (int i = 0; i < iter->n; i++) {
        for (int j = 0; j < iter->n; j++) {
            hessiana_evaluada[i][j] = evaluator_evaluate(iter->hessiana[i][j],
                                                         iter->n,
                                                         iter->nomes_vars,
                                                         iter->X);
        }
    }

    iter->i++;

    double *delta = resolver_sistema_gauss_seidel(hessiana_evaluada, gradiente_evaluado);

    free(gradiente_evaluado);
    free(hessiana_evaluada);

    for (int i = 0; i < iter->n; i++) {
        iter->X[i] += delta[i];
    }


    if (norma(delta, iter->n) < iter->epsilon) {
        iter->acabou = true;
        free(delta);
        return;
    }

    free(delta); // B)
} */