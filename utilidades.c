#include "utilidades.h"

// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void print_vetor_double(double v[], int tam)
{
    printf("[");

    for (int i = 0; i < tam - 1; i++)
        printf("%g, ", v[i]);

    printf("%g]\n", v[tam - 1]);
}

void print_matriz(double **M, int tam) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%g, ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void inicializa_matriz(double M[3][3], double **chata) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            chata[i][j] = M[i][j];
        }
    }
}

// TODO: erro malloc
void **cria_matriz(size_t size, int n)
{
    void **mat = malloc(n * sizeof(void *));

    for (int i = 0; i < n; i++)
        mat[i] = malloc(n * size);

    return mat;
}

void **destroi_matriz(void **mat, int n)
{
    for (int i = 0; i < n; i++)
        free(mat[i]);

    free(mat);
}

// TODO: erro malloc/calloc
void *cria_vetor(size_t size, int n)
{
    return calloc(n, size);
}

