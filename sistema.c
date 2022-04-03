#include "sistema.h"

#include "utilidades.h"

#include <math.h>
#include <stdlib.h>

typedef struct SistemaLinear {
    int n;
    double *b, *X;

    union {
        double **U;
        double **M;
    };
    
    double **L;

} SistemaLinear;

typedef struct ConfigLU {
    int *trocas;
} ConfigLU;

typedef struct ConfigGaussSeidel {
    double *X_old, *delta;
} ConfigGaussSeidel;

SistemaLinear* alocar_sl(int n) {
    SistemaLinear *sl = malloc(sizeof(SistemaLinear));

    sl->n = n;
    sl->b = criar_vetor(sizeof(double), n);
    sl->X = criar_vetor(sizeof(double), n);
    sl->M = (double**) criar_matriz(sizeof(double), n);
    sl->L = NULL;

    return sl;
}

void criar_sl(SistemaLinear *sl, double** M, double *b) {
    copiar_matriz_double(sl->M, M, sl->n);
    copiar_vetor_double(sl->b, b, sl->n);
}

static void retrossubs(double **M, double *X, double *b, int n) {
    for (int i = n - 1; i >=0; --i) {
        X[i] = b[i];

        for (int j = i + 1; j < n; j++)
            X[i] -= M[i][j] * X[j];

        // TODO : divisao por 0
        // if (fabs(M[i][i]) < 0.01)
        //     debug_print("%g", M[i][i]);
            
        X[i] /= M[i][i];
    }
}

static int encontra_max(SistemaLinear *sl, int coluna) {
    int max = coluna;

    for (int i = coluna; i < sl->n; i++) {
        if (fabs(sl->M[i][coluna]) > fabs(sl->M[max][coluna]))
            max = i;
    }

    return max;
}

static void troca_linha(SistemaLinear *sl, int linha_a, int linha_b) {
    double *tmp = sl->M[linha_a];
    sl->M[linha_a] = sl->M[linha_b];
    sl->M[linha_b] = tmp;

    double tmp2 = sl->b[linha_a];
    sl->b[linha_a] = sl->b[linha_b];
    sl->b[linha_b] = tmp2;
}

static void triangularizar(SistemaLinear *sl) {
    /* para cada linha a partir da primeira */
    for (int i = 0; i < sl->n; ++i) {

        int linha_pivo = encontra_max(sl, i);

        if (i != linha_pivo)
            troca_linha(sl, i, linha_pivo);

        for (int k = i + 1; k < sl->n; ++k) {
            // TODO: checar divisao por zero
            if (fabs(sl->M[i][i]) < 0.01)
                debug_print("%g", sl->M[i][i]);

            double m = sl->M[k][i] / sl->M[i][i];

            sl->M[k][i] = 0.0;

            for (int j = i + 1; j < sl->n; ++j)
                sl->M[k][j] -= sl->M[i][j] * m;

            sl->b[k] -= sl->b[i] * m;
        }
    }
}


void resolver_sl_eliminacao_gauss(SistemaLinear *sl) {
    triangularizar(sl);
    retrossubs(sl->M, sl->X, sl->b, sl->n);
}

static void retrossubs_L(double **M, double *Y, double *b, int n)
{
    for (int i = 0; i < n; ++i) {
        Y[i] = b[i];

        for (int j = i - 1; j >= 0; j--)
            Y[i] -= M[i][j] * Y[j];

        // TODO: divisao por 0
        // if (fabs(M[i][i]) < 0.01)
        //     debug_print("%g", M[i][i]);

        Y[i] /= M[i][i];
    }
}

static void troca_linha_LU(SistemaLinear *sl, ConfigLU *s, int linha_a, int linha_b) {
    double *tmp = sl->U[linha_a];
    sl->U[linha_a] = sl->U[linha_b];
    sl->U[linha_b] = tmp;

    tmp = sl->L[linha_a];
    sl->L[linha_a] = sl->L[linha_b];
    sl->L[linha_b] = tmp;

    int tmp_2 = s->trocas[linha_a];
    s->trocas[linha_a] = s->trocas[linha_b];
    s->trocas[linha_b] = tmp_2;
}

static void aplicar_trocas_LU(SistemaLinear *sl, ConfigLU *s) {
    double res[sl->n];

    for (int i = 0; i < sl->n; i++)
        res[i] = sl->b[s->trocas[i]];

    copiar_vetor_double(sl->b, res, sl->n);
}

static void fatorar_LU(SistemaLinear *sl, ConfigLU *s) {
    
    /* para cada linha a partir da primeira */
    for (int i = 0; i < sl->n; ++i) {

        int linha_pivo = encontra_max(sl, i);
        
        if (i != linha_pivo)
            troca_linha_LU(sl, s, i, linha_pivo);
            
        sl->L[i][i] = 1.0f;

        for (int k = i + 1; k < sl->n; ++k) {
            // TODO: checar divisao por zero
            // if (fabs(sl->U[i][i]) < 0.01)
            //     debug_print("%g", U[i][i]);
            

            double m = sl->U[k][i] / sl->U[i][i];

            sl->L[k][i] = m;
            sl->U[k][i] = 0.0;

            for (int j = i + 1; j < sl->n; ++j)
                sl->U[k][j] -= sl->U[i][j] * m;
        }
    }
    
    // print_matriz(sl->L, sl->n);
    // print_matriz(sl->U, sl->n);
}

void resolver_sl_LU(SistemaLinear *sl, ConfigLU *s) {
    aplicar_trocas_LU(sl, s);
    
    retrossubs_L(sl->L, sl->X, sl->b, sl->n);
    retrossubs(sl->U, sl->X, sl->X, sl->n);
}

void destruir_sl(SistemaLinear *sl) {
    destruir_matriz((void **)sl->M, sl->n);
    destruir_vetor(sl->b);
    destruir_vetor(sl->X);

    if(sl->L)
        destruir_matriz((void **)sl->L, sl->n);

    free(sl);
}

void resolver_sl_gauss_seidel(SistemaLinear *sl, ConfigGaussSeidel *s) {
    for (int iters = 0; iters < MAX_ITERS_GAUSS_SEIDEL; iters++) {
        
        for (int i = 0; i < sl->n; i++) {
            s->X_old[i] = sl->X[i];
            sl->X[i] = sl->b[i];

            for (int j = 0; j < i; j++) {
                sl->X[i] -= sl->M[i][j] * sl->X[j];
            }

            for (int j = i + 1; j < sl->n; j++) {
                sl->X[i] -= sl->M[i][j] * sl->X[j];
            }
            
            sl->X[i] /= sl->M[i][i];
            
            s->delta[i] = sl->X[i] - s->X_old[i];
        }

        if (norma(s->delta, sl->n) < 1e-6) {
            break;
        }
    }
}

ConfigLU *alocar_config_LU(SistemaLinear *sl) {
    ConfigLU *config = malloc(sizeof(ConfigLU));
    config->trocas = criar_vetor(sizeof(double), sl->n);
    sl->L = (double **) criar_matriz(sizeof(double), sl->n);

    return config;
}

void criar_config_LU(ConfigLU *config, SistemaLinear *sl) {

    for (int i = 0; i < sl->n; i++)
        config->trocas[i] = i;

    fatorar_LU(sl, config);
}

ConfigGaussSeidel *alocar_config_gauss_seidel(SistemaLinear *sl) {
    ConfigGaussSeidel *config = malloc(sizeof(ConfigGaussSeidel));

    config->delta = criar_vetor(sizeof(double), sl->n);
    config->X_old = criar_vetor(sizeof(double), sl->n);

    return config;
}

void criar_config_gauss_seidel(ConfigGaussSeidel *config, SistemaLinear *sl) {}

double *solucao_sl(SistemaLinear *sl) {
    return sl->X;
}

void destruir_config_LU(ConfigLU *s) {
    destruir_vetor(s->trocas);
    free(s);
}

void destruir_config_gauss_seidel(ConfigGaussSeidel *s) {
    destruir_vetor(s->delta);
    destruir_vetor(s->X_old);
    free(s);
}