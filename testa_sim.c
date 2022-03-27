#include "sim.h"
#include "newton.h"
#include <stdio.h>

void print_vetor_double(double v[], int tam)
{
    printf("[");

    for (int i = 0; i < tam - 1; i++)
        printf("%g, ", v[i]);

    printf("%g]\n", v[tam - 1]);
}

void testa_resolver_sistema()
{
    double **M = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}};

    double b[] = {1, 2, 3};

    double *X = resolver_sistema(M, b, 3);

    print_vetor_double(X, 3);
}

void main()
{
    testa_resolver_sistema();
}