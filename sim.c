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

        if (abs(M[i][i]) < 0.01)
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
        debug_print("%d", i);
        printf("Antes trocar\n");
        print_matriz(M, n);

        int linha_pivo = encontra_max(M, i, n);
        debug_print("%d", linha_pivo);

        if (i != linha_pivo)
            troca_linha(M, b, n, i, linha_pivo);
        
        printf("matriz com linha trocada\n");
        print_matriz(M, n);

        for (int k = i + 1; k < n; ++k) {
            debug_print("%d", i);
            debug_print("%d", k);
            print_matriz(M, n);
            // TODO: checar divisao por zero
            if (abs(M[i][i]) < 0.01)
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

double *resolver_sistema_LU(double **M, double *b, int n) {

}

double *resolver_sistema_gauss_seidel(double **M, double *b, int n) {

}