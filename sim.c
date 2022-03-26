#include "sim.h"

#include <stdlib.h>

static void retrossubs(double **M, double *X, double *b, int n) {
    for (int i = n - 1; i >=0; --i) {
        X[i] = b[i];

        for (int j = i + 1; j < n; j++)
            X[i] -= M[i][j] * X[j];

        X[i] /= M[i][i];
    }
}

static int encontra_max(double **M, int coluna, int n) {
    int max = 0;

    for (int i = 1; i < n; i++) {
        if (M[i][coluna] > M[max][coluna])
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
    for (int i= 0; i < n; ++i) {
        int linha_pivo = encontra_max(M, i, n);

        if (i != linha_pivo)
            troca_linha(M, b, n, i, linha_pivo);

        for (int k = i + 1; k < n; ++k) {
            // TODO: checar divisao por zero
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