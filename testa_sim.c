#include "sim.h"
#include "utilidades.h"
#include <stdio.h>

void testa_resolver_sistema()
{
    double **M = (double **) cria_matriz(sizeof(double), 3);
    inicializa_matriz((double[3][3]){{2, 5, 3}, {1, 2, 2}, {1, 1, 1}}, M);

    printf("Matriz chata antes:\n");
    print_matriz(M, 3);
    
//    double **M = {
//         {1, 0, 0},
//         {0, 1, 0},
//         {0, 0, 1}};

    

    double b[] = {11, 9, 6};

    double *X = resolver_sistema(M, b, 3);
    printf("\nMatriz chata depois:\n");
    print_matriz(M, 3);

    print_vetor_double(X, 3);
}

void testa_LU() {
    double A[3][3] = {
        {25, 5, 1},
        {64, 8, 1},
        {144, 12 ,1}
    };

    int trocas[3];

    double **M = (double**) cria_matriz(sizeof(double), 3);
    double **L = (double**) cria_matriz(sizeof(double), 3);
    double **U = (double**) cria_matriz(sizeof(double), 3);
    inicializa_matriz(A, M);

    calcula_LU(L, U, trocas, M, 3);

    mat_mult_mat_print(L, U, 3);
    
    printf("trocas\n");
    for (int i = 0; i < 3; i++) {
        printf("%d, ", trocas[i]);
    }
    printf("\n");
}

void main()
{
    //testa_resolver_sistema();
    testa_LU();
}