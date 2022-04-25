#include "newton.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <matheval.h>
#include <math.h>

#include "sim.h"
#include "utilidades.h"
#include "utils.h"

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

    iter->hessiana_evaluada = (double **)cria_matriz(sizeof(double), n);

    iter->L = (double **)cria_matriz(sizeof(double), n);
    iter->U = (double **)cria_matriz(sizeof(double), n);

    iter->X = calloc(n, sizeof(double));
    memcpy(iter->X, chute_inicial, n * sizeof(double));
    iter->hess_steps = n;
    iter->epsilon = epsilon;
    iter->max_iteracoes = max_iteracoes;
    iter->acabou = false;
    iter->trocas = cria_vetor(sizeof(int), n);

    return iter;
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

    // TODO: double free no chute inicial.
    free(iter->X);

    free(iter);
}

iteracao *iterar_newton_padrao(iteracao *iter)
{
    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();

    double *gradiente_evaluado = (double *)cria_vetor(sizeof(double), iter->n);

    tempo_grad = timestamp();

    for (int i = 0; i < iter->n; i++)
    {
        gradiente_evaluado[i] = -evaluator_evaluate(iter->gradiente[i],
                                                    iter->n,
                                                    iter->nomes_vars,
                                                    iter->X);
        // debug_print("%f ", gradiente_evaluado[i]);
    }

    tempo_grad = timestamp() - tempo_grad;
    iter->tempo_derivadas += tempo_grad;

    double norma_evaluada = norma(gradiente_evaluado, iter->n);

    if (norma_evaluada < iter->epsilon)
    {
        free(gradiente_evaluado);
        iter->acabou = true;
        iter->i++;
        return NULL;
    }

    // TODO: hessiana_evaluada => iter->hessiana_evaluada
    double **hessiana_evaluada = (double **)cria_matriz(sizeof(double), iter->n);

    tempo_hess = timestamp();

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

    tempo_hess = timestamp() - tempo_hess;
    iter->tempo_derivadas += tempo_hess;

    iter->i++;

    tempo_SL = timestamp();

    double *delta = resolver_sistema(hessiana_evaluada, gradiente_evaluado, iter->n);

    tempo_SL = timestamp() - tempo_SL;
    iter->tempo_SL += tempo_SL;

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
        return NULL;
    }

    free(delta); // B)
    tempo_total = timestamp() - tempo_total;
    iter->tempo_total += tempo_total;

    return NULL;
}

iteracao *iterar_newton_modificado(iteracao *iter)
{
    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();

    double *gradiente_evaluado = (double *)cria_vetor(sizeof(double), iter->n);

    tempo_grad = timestamp();

    for (int i = 0; i < iter->n; i++)
    {
        gradiente_evaluado[i] = -evaluator_evaluate(iter->gradiente[i],
                                                    iter->n,
                                                    iter->nomes_vars,
                                                    iter->X);
        //debug_print("%f ", gradiente_evaluado[i]);
    }

    tempo_grad = timestamp() - tempo_grad;
    iter->tempo_derivadas += tempo_grad;

    double norma_evaluada = norma(gradiente_evaluado, iter->n);

    if (norma_evaluada < iter->epsilon)
    {
        free(gradiente_evaluado);
        iter->acabou = true;
        iter->i++;
        return NULL;
    }

    if (!(iter->i % iter->hess_steps))
    {
        tempo_hess = timestamp();

        for (int i = 0; i < iter->n; i++)
        {
            for (int j = 0; j < iter->n; j++)
            {
                iter->hessiana_evaluada[i][j] = evaluator_evaluate(iter->hessiana[i][j],
                                                                   iter->n,
                                                                   iter->nomes_vars,
                                                                   iter->X);
            }
        }

        calcula_LU(iter->L, iter->U, iter->trocas, iter->hessiana_evaluada, iter->n);

        tempo_hess = timestamp() - tempo_hess;
        iter->tempo_derivadas += tempo_hess;
    }

    iter->i++;

    tempo_SL = timestamp();

    double *delta = resolver_sistema_LU(iter->L, iter->U, gradiente_evaluado, iter->trocas, iter->n);

    tempo_SL = timestamp() - tempo_SL;
    iter->tempo_SL += tempo_SL;

    free(gradiente_evaluado);

    for (int i = 0; i < iter->n; i++)
    {
        iter->X[i] += delta[i];
    }

    if (norma(delta, iter->n) < iter->epsilon)
    {
        iter->acabou = true;
        free(delta);
        return NULL;
    }

    free(delta); // B)

    tempo_total = timestamp() - tempo_total;
    iter->tempo_total += tempo_total;

    return NULL;
}

iteracao* iterar_newton_inexato(iteracao* iter) {
    double tempo_total, tempo_grad, tempo_hess, tempo_SL;

    tempo_total = timestamp();

    double *gradiente_evaluado = (double *)cria_vetor(sizeof(double), iter->n);

    tempo_grad = timestamp();

    for (int i = 0; i < iter->n; i++)
    {
        gradiente_evaluado[i] = -evaluator_evaluate(iter->gradiente[i],
                                                    iter->n,
                                                    iter->nomes_vars,
                                                    iter->X);
        // debug_print("%f ", gradiente_evaluado[i]);
    }

    tempo_grad = timestamp() - tempo_grad;
    iter->tempo_derivadas += tempo_grad;

    double norma_evaluada = norma(gradiente_evaluado, iter->n);

    if (norma_evaluada < iter->epsilon)
    {
        free(gradiente_evaluado);
        iter->acabou = true;
        iter->i++;
        return NULL;
    }

    // TODO: hessiana_evaluada => iter->hessiana_evaluada
    double **hessiana_evaluada = (double **)cria_matriz(sizeof(double), iter->n);

    tempo_hess = timestamp();

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

    tempo_hess = timestamp() - tempo_hess;
    iter->tempo_derivadas += tempo_hess;

    iter->i++;

    tempo_SL = timestamp();

    double *delta = resolver_sistema_gauss_seidel(hessiana_evaluada, gradiente_evaluado, iter->n);

    tempo_SL = timestamp() - tempo_SL;
    iter->tempo_SL += tempo_SL;

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
        return NULL;
    }

    free(delta); // B)

    tempo_total = timestamp() - tempo_total;
    iter->tempo_total += tempo_total;

    return NULL;
}
