/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/
#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>

#include "criticante.h"

/**
 * @brief Lê dados de dados do stdin.
 * 
 * @param n Referência para o número de variáveis.
 * @param f_str String com a expressão da função.
 * @param max_iters Referẽncia para o valor máximo de iterações.
 * @param epsilon Referência para o erro máximo.
 * @param X0 Ponteiro para vetor do chute inicial.
 * @return int Sucesso retorna 0. Em caso de erro retorna -1.
 */
int ler_dados_criticantes(int *n, char **f_str, int *max_iters, double *epsilon, double **X0);

/**
 * @brief Escreve o cabeçalho para o arquivo especificado.
 * 
 * @param f Arquivo em que será escrito.
 * @param n Número de variáveis.
 * @param f_str String contendo a expressão da função.
 * @return int Sempre retorna 0.
 */
int escrever_cabecalho(FILE *f, int n, char *f_str);

/**
 * @brief Escreve os dados de uma iteração no formato especificado.
 * 
 * @param f Arquivo no qual será escrito.
 * @param padrao Informação da iteração do tipo padrão.
 * @param modificado Informação da iteração do tipo modificado.
 * @param inexato Informação da iteração do tipo inexato.
 * @return int Sempre retorna 0.
 */
int escrever_dados(FILE *f, IterInfo padrao, IterInfo modificado, IterInfo inexato);

/**
 * @brief Escreve os tempos no formato especificado.
 * 
 * @param f Arquivo no qual será escrito.
 * @param padrao Informação da iteração do tipo padrão.
 * @param modificado Informação da iteração do tipo modificado.
 * @param inexato Informação da iteração do tipo inexato.

 * @return int 
 */
int escrever_tempos(FILE *f, IterInfo padrao, IterInfo modificado, IterInfo inexato);

#endif
