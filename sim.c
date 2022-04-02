#include "sim.h"
#include "utilidades.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static void retrossubs(double **M, double *X, double *b, int n) {
    for (int i = n - 1; i >=0; --i) {
        X[i] = b[i];

        for (int j = i + 1; j < n; j++)
            X[i] -= M[i][j] * X[j];

        if (fabs(M[i][i]) < 0.01)
            debug_print("%g", M[i][i]);
            
        X[i] /= M[i][i];
    }
}

static int encontra_max(double **M, int coluna, int n) {
    int max = coluna;

    for (int i = coluna; i < n; i++) {
        if (fabs(M[i][coluna]) > fabs(M[max][coluna]))
            max = i;
    }

    return max;
}

static void troca_linha(double **M, double *b, int n, int linha_a, int linha_b) {
    double *tmp = M[linha_a];
    M[linha_a] = M[linha_b];
    M[linha_b] = tmp;

    double tmp2 = b[linha_a];
    b[linha_a] = b[linha_b];
    b[linha_b] = tmp2;
}

static void triangularizar(double **M, double *b, int n ) {
    /* para cada linha a partir da primeira */
    for (int i = 0; i < n; ++i) {

        int linha_pivo = encontra_max(M, i, n);

        if (i != linha_pivo)
            troca_linha(M, b, n, i, linha_pivo);

        for (int k = i + 1; k < n; ++k) {
            // TODO: checar divisao por zero
            if (fabs(M[i][i]) < 0.01)
                debug_print("%g", M[i][i]);

            double m = M[k][i] / M[i][i];

            M[k][i] = 0.0;

            for (int j = i + 1; j < n; ++j)
                M[k][j] -= M[i][j] * m;

            b[k] -= b[i] * m;
        }
    }
}


double *resolver_sistema(double **M, double *b, int n) {
    double *X = calloc(n, sizeof(double));
    
    triangularizar(M, b, n);

    retrossubs(M, X, b, n);
    
    return X;
}

void calcula_LU(double **L, double **U, int *trocas, double ** M, int n) {
    double *__inutil_tirar_TODO = calloc(n, sizeof(double)); 

    //printf("calcula_LU() -> M:\n");
    //print_matriz(M, n);

    for (int i = 0; i < n; i++) {
        trocas[i] = i;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            U[i][j] = M[i][j];
        }
        
    }
    
    for (int i = 0; i < n; i++) {
        int linha_pivo = encontra_max(U, i, n);
        
        if (i != linha_pivo) {
            troca_linha(U, __inutil_tirar_TODO, n, i, linha_pivo);
            int tmp = trocas[i];
            trocas[i] = trocas[linha_pivo];
            trocas[linha_pivo] = tmp;
        }
    }
     

    /* para cada linha a partir da primeira */
    for (int i = 0; i < n; ++i) {

        L[i][i] = 1.0f;

        for (int k = i + 1; k < n; ++k) {
            // TODO: checar divisao por zero
            if (fabs(U[i][i]) < 0.01)
                debug_print("%g", U[i][i]);

            double m = U[k][i] / U[i][i];

            L[k][i] = m;
            
            U[k][i] = 0.0;

            for (int j = i + 1; j < n; ++j)
                U[k][j] -= U[i][j] * m;

        }
    }
    //print_matriz(L, n);
    //print_matriz(U, n);
    free(__inutil_tirar_TODO);
}

static void retrossubs_bugada(double **M, double *X, double *b, int n)
{
    for (int i = 0; i < n; ++i) {
        X[i] = b[i];

        for (int j = i - 1; j >= 0; j--)
            X[i] -= M[i][j] * X[j];

        if (fabs(M[i][i]) < 0.01)
            debug_print("%g", M[i][i]);

        X[i] /= M[i][i];
    }
}

double *resolver_sistema_LU(double **L, double **U, double *b, int* trocas, int n) {
    double *X = calloc(n, sizeof(double));
    double *Y = calloc(n, sizeof(double));
    
    for (int i = 0; i < n; i++) {
        double tmp = b[i];
        b[i] = b[trocas[i]];
        b[trocas[i]] = tmp;
    }

    retrossubs_bugada(L, Y, b, n);
    retrossubs(U, X, Y, n);

    free(Y);
    return X;
}

double *resolver_sistema_gauss_seidel(double **M, double *b, int n) {    
    double *X = calloc(n, sizeof(double));
    double *X_old = calloc(n, sizeof(double));
    double *delta = calloc(n, sizeof(double));
    const int max_iters = 50;
    
    //print_matriz(M, n);
    

    for (int iters = 0; iters < max_iters; iters++) {
        
        for (int i = 0; i < n; i++) {
            X_old[i] = X[i];
            X[i] = b[i];

            for (int j = 0; j < i; j++) {
                X[i] -= M[i][j] * X[j];
            }

            for (int j = i + 1; j < n; j++) {
                X[i] -= M[i][j] * X[j];
            }
            
            X[i] /= M[i][i];
            
            delta[i] = X[i] - X_old[i];
        }

        if (norma(delta, n) < 1e-6) {
            break;
        }
    }

    free(X_old);
    free(delta);

    return X;
}
