#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stddef.h>

#define debug_print(fmt, var) \
    printf("%s:%d " #var " = " fmt "\n", __FILE__, __LINE__, (var))

void print_vetor_double(double v[], int tam);

void print_matriz(double **M, int tam);

void inicializa_matriz(double M[3][3], double **chata);

void **cria_matriz(size_t size, int n);

void **destroi_matriz(void **mat, int n);

void *cria_vetor(size_t size, int n);

void mat_mult_mat_print(double **A, double **B, int n);

double norma(double *X, int n);

#endif