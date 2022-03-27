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

void main()
{
    testa_resolver_sistema();
}