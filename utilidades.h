#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stddef.h>
#include <stdio.h>

#define debug_print(fmt, var) \
    printf("%s:%d " #var " = " fmt "\n", __FILE__, __LINE__, (var))

void print_vetor_double(double v[], int tam);

void print_matriz(double **M, int tam);

void inicializa_matriz(double M[3][3], double **chata);

void **criar_matriz(size_t size, int n);

void copiar_matriz_double(double **dst, double **src, int n);

void **destruir_matriz(void **mat, int n);

void *criar_vetor(size_t size, int n);

void destruir_vetor(void *v);

void copiar_vetor_double(double *dst, double *src, int n);

void mat_mult_mat_print(double **A, double **B, int n);

double norma(double *X, int n);

#endif