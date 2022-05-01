/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/
#ifndef SISTEMA_H
#define SISTEMA_H

#include "utilidades.h"

#define MAX_ITERS_GAUSS_SEIDEL 50

/**
 * @brief Estrutura que guarda dados fundamentais para resolver o sistema com qualquer método.
 * 
 */
typedef struct SistemaLinear SistemaLinear;

/**
 * @brief Estrutura que guarda dados necessários para resolver o sistema com fatoração LU.
 * 
 */
typedef struct ConfigLU ConfigLU;

/**
 * @brief Estrutura que guarda dados necessários para resolver o sistema com o método de Gauss-Seidel.
 * 
 */
typedef struct ConfigGaussSeidel ConfigGaussSeidel;

/**
 * @brief Aloca espaço na memória para a estrutura SistemaLinear.
 * 
 * @param n Número de variáveis do sistema. 
 * @return SistemaLinear* NULL caso haja alguma falha na alocação e o SistemaLinear criado em caso de sucesso.
 */
SistemaLinear* alocar_sl(int n);

/**
 * @brief Inicializa o SistemaLinear com a matriz de coeficientes e os termos independentes.
 * 
 * @param sl Um SistemaLinear já alocado.
 * @param M A matriz de coeficientes, de tamanho nxn.
 * @param b Termos independentes do sistema linear, de tamanho n.
 */
void criar_sl(SistemaLinear *sl, MatrizOptDouble *M, double *b);

/**
 * @brief Muda a matriz de coeficientes de um SistemaLinear.
 * 
 * @param sl Um SistemaLinear já alocado.
 * @param M A matriz de coeficientes, de tamanho nxn.
 */
// void setar_matriz_sl(SistemaLinear *sl, double *M);

/**
 * @brief Muda os termos independentes de um SistemaLinear.
 * 
 * @param sl Um SistemaLinear já alocado.
 * @param b Termos independentes do sistema linear, de tamanho n.
 */
// void setar_termos_independentes_sl(SistemaLinear *sl, double *b);

/**
 * @brief Libera a memória de um SistemaLinear.
 * 
 * @param sl Um SistemaLinear já alocado.
 */
void destruir_sl(SistemaLinear *sl);

/**
 * @brief Resolve um sistema usando o método de eliminação de Gauss.
 * 
 * @param sl Um SistemaLinear já alocado e já criado.
 */
void resolver_sl_eliminacao_gauss(SistemaLinear *sl);

/**
 * @brief Resolve um sistema usando o método de eliminação de Gauss com fatoração LU.
 * 
 * @param sl Um SistemaLinear já alocado, já criado e com config já criada.
 * @param s Uma configLU já alocada e criada.
 */
void resolver_sl_LU(SistemaLinear *sl, ConfigLU *s);

/**
 * @brief Resolve um sistema linear usando o método de Gauss-Seidel.
 * 
 * @param sl Um SistemaLinear já alocado, já criado e com config já criada.
 * @param s Uma ConfigGaussSeidel já alocada e já criada.
 */
void resolver_sl_gauss_seidel(SistemaLinear *sl, ConfigGaussSeidel *s);

/**
 * @brief Aloca espaço na memória para a estrutura ConfigLU e espaço adicional no SistemaLinear.
 * 
 * @param sl SistemaLinear em qual vai ser alocada memória e para qual a config será criada.
 * @return ConfigLU* NULL caso haja alguma falha na alocação e a ConfigLU criada em caso de sucesso.
 */
ConfigLU *alocar_config_LU(SistemaLinear *sl);

/**
 * @brief Inicializa a ConfigLU e fatora a matriz de coefientes de `sl` em duas.
 * 
 * @param config ConfigLU já alocada.
 * @param sl Um SistemaLinear já alocado e com sua config já alocada.
 */
void criar_config_LU(ConfigLU *config, SistemaLinear *sl);

/**
 * @brief Inicializa a ConfigLU e fatora a matriz de coefientes de `sl` em duas.
 * 
 * @param config ConfigLU já alocada.
 * @param sl Um SistemaLinear já alocado e com sua config já alocada.
 */
void criar_config_gauss_seidel(ConfigGaussSeidel *config, SistemaLinear *sl);

/**
 * @brief Aloca espaço na memória para uma ConfigGaussSeidel.
 * 
 * @param sl SistemaLinear para o qual será alocado a config.
 * @return ConfigLU* NULL caso haja alguma falha na alocação e a ConfigLU criada em caso de sucesso.
 */
ConfigGaussSeidel *alocar_config_gauss_seidel(SistemaLinear *sl);

/**
 * @brief Não faz nada. Ou inicializa a config, que é equivalente a fazer nada.
 * 
 * @param config nada.
 * @param sl nada.
 */
void criar_config_gauss_seidel(ConfigGaussSeidel *config, SistemaLinear *sl);

/**
 * @brief Libera espaço da memória de uma ConfigLU.
 * 
 * @param s A ConfigLU já alocada a ser destruída.
 */
void destruir_config_LU(ConfigLU *s);

/**
 * @brief Libera espaço da memória de uma ConfigGaussSeidel.
 * 
 * @param s A ConfigGaussSeidel já alocada a ser destruída.
 */
void destruir_config_gauss_seidel(ConfigGaussSeidel *s);

/**
 * @brief Obtêm a solução de um SistemaLinear resolvido.
 * 
 * @param sl SistemaLinear já resolvido.
 * @return double* Vetor contendo a solução do sl. Não dar free().
 */
double *solucao_sl(SistemaLinear *sl);

#endif
