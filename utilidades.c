#include "utilidades.h"

// #include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void print_vetor_double(double v[], int tam)
{
    printf("[");

    for (int i = 0; i < tam - 1; i++)
        printf("%g, ", v[i]);

    printf("%g]\n", v[tam - 1]);
}

void print_matriz(double **M, int tam) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
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

void **criar_matriz(size_t size, int n)
{
    void **mat = calloc(n, sizeof(void *));
    if(!mat) return NULL;

    for (int i = 0; i < n; i++)
        if(!(mat[i] = calloc(n, size))) return NULL;

    return mat;
}

void copiar_matriz_double(double **dst, double **src, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[i][j] = src[i][j];    
}

void destruir_matriz(void **mat, int n)
{
    for (int i = 0; i < n; i++)
        free(mat[i]);

    free(mat);
}

// TODO: erro malloc/calloc
void *criar_vetor(size_t size, int n)
{
    return calloc(n, size);
}

void destruir_vetor(void *v) {
    free(v);
}

void copiar_vetor_double(double *dst, double *src, int n) {
    memcpy(dst, src, sizeof(double) * n);
}

void mat_mult_mat_print(double **A, double **B, int n) {
    double soma = 0;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            soma = 0;
            for (int k = 0; k < n; k++) {
                soma += A[i][k] * B[k][j];
            }
            printf("%g, ", soma);
        }
        printf("\n");
    }
}

double norma(double *X, int n)
{
    double max = fabs(X[0]);
    for (int i = 1; i < n; i++)
    {
        if (fabs(X[i]) > max)
            max = fabs(X[i]);
    }

    return max;
}

void somar_vetor(double *dst, double *src, int n) {
    for (int i = 0; i < n; i++)
        dst[i] += src[i];
}