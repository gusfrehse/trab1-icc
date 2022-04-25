/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/
#ifndef CRITICANTE_H
#define CRITICANTE_H

#include <stdbool.h>

/**
 * @brief Informações sobre uma iteração do criticante.
 * 
 */
typedef struct IterInfo {
    int iteracao;
    double tempo_total;
    double tempo_derivadas;
    double tempo_SL;
    double f_x;
    bool acabou;
    bool ocorreu_erro;
} IterInfo;

/**
 * @brief Tipos de criticantes possíveis, define, principalmente, o método que
 * será usado para resolver os sistemas lineares envolvidos.
 * 
 */
typedef enum TipoCriticante {
    CRITICANTE_PADRAO,
    CRITICANTE_MODIFICADO,
    CRITICANTE_INEXATO
} TipoCriticante;

/**
 * @brief Estrutura usada para achar pontos críticos de uma função com entradas
 * multivariadas com o método de newton iterativo.
 * 
 */
typedef struct Criticante Criticante;

/**
 * @brief Aloca espaço na memória e inicialiaza uma estrutura criticante.
 * 
 * @param n Dimensão da função
 * @param f_str String que representa a função.
 * @param max_iters Máximo de iterações.
 * @param epsilon Valor máximo de erro permitido.
 * @param X0 Chute inicial, já alocado e inicializado.
 * @param tipo Tipo de criticante a ser criado.
 * @return Criticante* 
 */
Criticante *criar_criticante(int n, char* f_str, int max_iters, double epsilon, double *X0, TipoCriticante tipo);

/**
 * @brief Executa uma iteração do criticante. Caso já tenha passado do máximo de
 * iterações ou achado uma solução, não faz nada.
 * 
 * @param c Criticantes a ser iterado.
 * @return IterInfo Informações sobre a iteração realizada.
 */
IterInfo iterar_criticante(Criticante *c);

/**
 * @brief Checa se o criticante já passou do máximo de iterações ou se já achou
 * uma solução.
 * 
 * @param crit Criticante a ser checado.
 * @return true Se já passou do máximo de iterações ou se já achou uma solução.
 * @return false Se não passou do máximo de iterações nem achou uma solução.
 */
bool acabou_criticante(Criticante *crit);

/**
 * @brief Libera a memória utilizada pelo Criticante.
 * 
 * @param crit Criticante a ser destruído.
 */
void destruir_criticante(Criticante *crit);

/**
 * @brief Retorna a possível solução encontrada pelo Criticante atualmente.
 * 
 * @param c Criticante que tem a possível solução.
 * @return double* Vetor com a solução. Não dar free.
 */
double *solucao_criticante(Criticante* c);

#endif
