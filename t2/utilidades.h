/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/
#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <stddef.h>
#include <stdio.h>


/**
 * @brief Printa o valor de uma variável da tela. Utilizada para debug
 * 
 * @param especificador da variável no printf
 * @param var variável
 */
#define debug_print(fmt, var) \
    printf("%s:%d " #var " = " fmt "\n", __FILE__, __LINE__, (var))

/**
 * @brief Printa vetor de double. Utilizada para debug
 * 
 * @param v Vetor
 * @param tam Tamanho
 */
void print_vetor_double(double v[], int tam);

/**
 * @brief Printa matriz de double
 * 
 * @param M Matriz
 * @param tam Tamanho
 */
void print_matriz(double **M, int tam);

/**
 * @brief Criam matriz nxn
 * 
 * @param size tamanho em bytes das células
 * @param n dimensã da matriz
 * @return void** 
 */
void **criar_matriz(size_t size, int n);

/**
 * @brief Copia elementos de uma matriz de double nxn para outra matriz de double nxn
 * 
 * @param dst Destino
 * @param src Fonte
 * @param n Dimensão da matriz
 */
void copiar_matriz_double(double **dst, double **src, int n);

/**
 * @brief elimina matriz alocada da memória pela função criar_matriz()
 * 
 * @param mat Matriz
 * @param n Dimensão
 */
void destruir_matriz(void **mat, int n);

/**
 * @brief Cria vetor de tamanho n
 * 
 * @param size Tamanho em bytes da célula
 * @param n dimensão do vetor
 * @return void* 
 */
void *criar_vetor(size_t size, int n);

/**
 * @brief Libera da memória o vetor alocado pela função criar_vetor()
 * 
 * @param v vetor
 */
void destruir_vetor(void *v);

/**
 * @brief Copia vetor de double de dimensão n para outro vetor de double de dimensão n
 * 
 * @param dst Destino
 * @param src Fonte
 * @param n Dimensão
 */
void copiar_vetor_double(double *dst, double *src, int n);

/**
 * @brief Soma dois vetores e armazena o resultado em dst
 * 
 * @param dst Destino
 * @param src Fonte
 * @param n Dimensão dos vetores
 */
void somar_vetor(double *dst, double *src, int n);

/**
 * @brief Calcula norma de vetor (maior elemento).
 * 
 * @param X Vetor
 * @param n Dimensão
 * @return double 
 */
double norma(double *X, int n);

#endif