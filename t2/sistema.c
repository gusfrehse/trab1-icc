/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/
#include "sistema.h"

#include "utilidades.h"

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Epsilon para checagem de erro por divisão por 0.
#define EPSILON_ZERO 1e-6


/**
 * @brief Estrutura que guarda dados fundamentais para resolver o sistema com qualquer método.
 * 
 */
typedef struct SistemaLinear {
    int n;
    double *b, *X;

    union {
        MatrizOptDouble U;
        MatrizOptDouble M;
    };
    
    MatrizOptDouble L;
    
    bool ocorreu_erro;
} SistemaLinear;

/**
 * @brief Armazena configurações necessárias para o algoritmo da fatoração LU
 * Além do Sistema linear, precisamos guardar adicionalmente um vetor de trocas
 * para aplicar o pivoteamento parcial em b.
 * 
 */
typedef struct ConfigLU {
    int *trocas;
} ConfigLU;


/**
 * @brief Armazena configurações necessárias para o algoritmo Gauss Seidel
 * 
 */
typedef struct ConfigGaussSeidel {
    double *X_old, *delta;
} ConfigGaussSeidel;


/**
 * @brief Triangulariza um sistema linear. utilizado na eliminação de Gauss.
 * 
 * @param sl Sistema linear
 * @return int 
 */
static int triangularizar(SistemaLinear *sl);

/**
 * @brief Realiza retrosubstituição em um sistema previamente triangularizado.
 * 
 * @param M Matriz dos coeficientes lineares
 * @param X Vetor X de resposta
 * @param b Vetor de coeficientes independentes
 * @param n Tamanho do sistema
 * @return int Se ocorrer algum erro retorna -1. Caso contrário retorna 0.
 */
static int retrossubs(MatrizOptDouble M, double *X, double *b, int n);

/**
 * @brief Troca duas linhas de um sistema linear.
 * 
 * @param sl Sistema Linear
 * @param linha_a Primeira linha
 * @param linha_b Segudnda linha
 */
static void troca_linha(SistemaLinear *sl, int linha_a, int linha_b);

/**
 * @brief Encontra o índice da linha com o maior elemento na coluna "coluna"
 * de um sistema linear
 * 
 * @param sl Sistema Linear
 * @param coluna Coluna a ser considerada
 * @return int 
 */
static int encontra_max(SistemaLinear *sl, int coluna);


/**
 * @brief Realiza a fatoração LU de um Sistema Linear.
 * 
 * @param sl Sistema Linear
 * @param s Configurações do Algoritmo Fatoração LU
 * @return int 
 */
//static int fatorar_LU(SistemaLinear *sl, ConfigLU *s);

/**
 * @brief Realiza a retrosubstituição em uma matriz triangular inferior.
 * 
 * @param M Matriz de coeficientes lineares
 * @param Y Vetor de resultados
 * @param b Vetor de termos independentes
 * @param n Tamanho do sistema
 * @return int 
 */
static int retrossubs_L(MatrizOptDouble M, double *Y, double *b, int n);

/**
 * @brief Troca linhas de um sistema linear fatorado.
 * 
 * @param sl Sistema Linear
 * @param s Configuração do Algoritmo Fatoração LU
 * @param linha_a Primeira linha
 * @param linha_b Segunda linha
 */
static void troca_linha_LU(SistemaLinear *sl, ConfigLU *s, int linha_a, int linha_b);

/**
 * @brief Aplica as trocas realizadas nas matrizes LU pelo pivoteamento parcial
 * no vetor de termos independentes.
 * 
 * @param sl Sistema Linear
 * @param s Configuração do Algoritmo Fatoração LU
 */
static void aplicar_trocas_LU(SistemaLinear *sl, ConfigLU *s);


SistemaLinear* alocar_sl(int n) {
    SistemaLinear *sl = malloc(sizeof(SistemaLinear));

    sl->n = n;
    sl->b = criar_vetor(sizeof(double), n);
    if(!sl->b) {
        free(sl);
        return NULL;
    }
        

    sl->X = criar_vetor(sizeof(double), n);
    if (!sl->X) {
        free(sl->b);
        free(sl);
        return NULL;
    }
    sl->M = (double*) criar_matriz_otimizada(sizeof(double), n);
    if(!sl->M) {
        free(sl->X);
        free(sl->b);
        free(sl);
        return NULL;
    }
    
    sl->L = NULL;
    sl->ocorreu_erro = false;

    return sl;
}

void criar_sl(SistemaLinear *sl, MatrizOptDouble M, double *b) {
    copiar_matriz_otimizada_double(sl->M, M, sl->n);
    copiar_vetor_double(sl->b, b, sl->n);
}

static int retrossubs(MatrizOptDouble M, double *X, double *b, int n) {
    for (int i = n - 1; i >=0; --i) {
        X[i] = b[i];

	double sub = 0.0;
	for (int j = i + 1; j < n; j++) {
        sub += EM(M, n, i, j) * X[j];
	}
	X[i] -= sub;

        if (fabs(EM(M, n, i, i)) < EPSILON_ZERO)
            return -1;
            
        X[i] /= EM(M, n, i, i);
    }
    
    return 0;
}

static int encontra_max(SistemaLinear *sl, int coluna) {
    int max = coluna;

    for (int i = coluna; i < sl->n; i++) {
        if (fabs(EM(sl->M, sl->n, i, coluna)) > fabs(EM(sl->M, sl->n, max, coluna)))
            max = i;
    }

    return max;
}

static void troca_linha(SistemaLinear *sl, int linha_a, int linha_b) {
    
    double tmp[sl->n];
    memcpy(tmp, EM_LINHA(sl->M, sl->n, linha_a), sl->n * sizeof(double));
    memcpy(EM_LINHA(sl->M, sl->n, linha_a), EM_LINHA(sl->M, sl->n, linha_b), sl->n * sizeof(double));
    memcpy(EM_LINHA(sl->M, sl->n, linha_b), tmp, sl->n * sizeof(double));

    double tmp2 = sl->b[linha_a];
    sl->b[linha_a] = sl->b[linha_b];
    sl->b[linha_b] = tmp2;
}

static int triangularizar(SistemaLinear *sl) {
    /* para cada linha a partir da primeira */
    for (int i = 0; i < sl->n; ++i) {

        int linha_pivo = encontra_max(sl, i);

        if (i != linha_pivo)
            troca_linha(sl, i, linha_pivo);

        for (int k = i + 1; k < sl->n; ++k) {
            // TODO: checar divisao por zero
            if (fabs(EM(sl->M, sl->n, i, i)) < EPSILON_ZERO)
                return -1;

            double m = EM(sl->M, sl->n, k, i) / EM(sl->M, sl->n, i, i);

            EM(sl->M, sl->n, k, i) = 0.0;

            for (int j = i + 1; j < sl->n; ++j)
                EM(sl->M, sl->n, k, j) -= EM(sl->M, sl->n, i, j) * m;

            sl->b[k] -= sl->b[i] * m;
        }
    }

    return 0;
}


void resolver_sl_eliminacao_gauss(SistemaLinear *sl) {
    triangularizar(sl);
    retrossubs(sl->M, sl->X, sl->b, sl->n);
}

//static int retrossubs_L(MatrizOptDouble M, double *Y, double *b, int n)
//{
//    for (int i = 0; i < n; ++i) {
//        Y[i] = b[i];
//
//        for (int j = i - 1; j >= 0; j--)
//            Y[i] -= M[i][j] * Y[j];
//
//        if (fabs(M[i][i]) < EPSILON_ZERO)
//            return -1;
//
//        Y[i] /= M[i][i];
//    }
//    
//    return 0;
//}
//
//static void troca_linha_LU(SistemaLinear *sl, ConfigLU *s, int linha_a, int linha_b) {
//    double *tmp = sl->U[linha_a];
//    sl->U[linha_a] = sl->U[linha_b];
//    sl->U[linha_b] = tmp;
//
//    tmp = sl->L[linha_a];
//    sl->L[linha_a] = sl->L[linha_b];
//    sl->L[linha_b] = tmp;
//
//    int tmp_2 = s->trocas[linha_a];
//    s->trocas[linha_a] = s->trocas[linha_b];
//    s->trocas[linha_b] = tmp_2;
//}
//
//static void aplicar_trocas_LU(SistemaLinear *sl, ConfigLU *s) {
//    double res[sl->n];
//
//    for (int i = 0; i < sl->n; i++)
//        res[i] = sl->b[s->trocas[i]];
//
//    copiar_vetor_double(sl->b, res, sl->n);
//}
//
//static int fatorar_LU(SistemaLinear *sl, ConfigLU *s) {
//    
//    /* para cada linha a partir da primeira */
//    for (int i = 0; i < sl->n; ++i) {
//
//        int linha_pivo = encontra_max(sl, i);
//        
//        if (i != linha_pivo)
//            troca_linha_LU(sl, s, i, linha_pivo);
//            
//        sl->L[i][i] = 1.0f;
//
//        for (int k = i + 1; k < sl->n; ++k) {
//            if (fabs(sl->M[i][i]) < EPSILON_ZERO)
//                return -1;
//            
//
//            double m = sl->U[k][i] / sl->U[i][i];
//
//            sl->L[k][i] = m;
//            sl->U[k][i] = 0.0;
//
//            for (int j = i + 1; j < sl->n; ++j)
//                sl->U[k][j] -= sl->U[i][j] * m;
//        }
//    }
//    
//    return 0;
//}
//
//void resolver_sl_LU(SistemaLinear *sl, ConfigLU *s) {
//    aplicar_trocas_LU(sl, s);
//    
//    retrossubs_L(sl->L, sl->X, sl->b, sl->n);
//    retrossubs(sl->U, sl->X, sl->X, sl->n);
//}

void destruir_sl(SistemaLinear *sl) {
    destruir_matriz_otimizada((MatrizOptDouble)sl->M);
    destruir_vetor(sl->b);
    destruir_vetor(sl->X);

    if(sl->L)
        destruir_matriz_otimizada((MatrizOptDouble)sl->L);

    free(sl);
}

void resolver_sl_gauss_seidel(SistemaLinear *sl, ConfigGaussSeidel *s) {
    for (int iters = 0; iters < MAX_ITERS_GAUSS_SEIDEL; iters++) {
        
        for (int i = 0; i < sl->n; i++) {
            s->X_old[i] = sl->X[i];
            sl->X[i] = sl->b[i];

			double soma = 0.0;
            for (int j = 0; j < i; j++) {
                soma += EM(sl->M, sl->n, i, j) * sl->X[j];
            }

            for (int j = i + 1; j < sl->n; j++) {
                soma += EM(sl->M, sl->n, i, j) * sl->X[j];
            }

			sl->X[i] -= soma;
            
            sl->X[i] /= EM(sl->M, sl->n, i, i);
            
            s->delta[i] = sl->X[i] - s->X_old[i];
        }

        if (norma(s->delta, sl->n) < 1e-6) {
            break;
        }
    }
}

//ConfigLU *alocar_config_LU(SistemaLinear *sl) {
//    ConfigLU *config = malloc(sizeof(ConfigLU));
//    if(!config) return NULL;
//
//    config->trocas = criar_vetor(sizeof(double), sl->n);
//    if(!config->trocas) {
//        free(config);
//        return NULL;
//    }
//
//    sl->L = (double *) criar_matriz_otimizada(sizeof(double), sl->n);
//    if(!sl->L) {
//        free(config->trocas);
//        free(config);
//        return NULL;
//    }
//
//    return config;
//}
//
//void criar_config_LU(ConfigLU *config, SistemaLinear *sl) {
//
//    for (int i = 0; i < sl->n; i++)
//        config->trocas[i] = i;
//
//    fatorar_LU(sl, config);
//}

ConfigGaussSeidel *alocar_config_gauss_seidel(SistemaLinear *sl) {
    ConfigGaussSeidel *config = malloc(sizeof(ConfigGaussSeidel));
    if(!config) return NULL;

    config->delta = criar_vetor(sizeof(double), sl->n);
    if(!config->delta) {
        free(config);
        return NULL;
    }
    config->X_old = criar_vetor(sizeof(double), sl->n);
    if(!config->X_old) {
        free(config->delta);
        free(config);
        return NULL;
    }

    return config;
}

void criar_config_gauss_seidel(ConfigGaussSeidel *config, SistemaLinear *sl) {}

double *solucao_sl(SistemaLinear *sl) {
    if (!sl->ocorreu_erro)
        return sl->X;
        
    return NULL;
}

void destruir_config_LU(ConfigLU *s) {
    destruir_vetor(s->trocas);
    free(s);
}

void setar_matriz_sl(SistemaLinear *sl, double *M) {
    copiar_matriz_otimizada_double(sl->M, M, sl->n);
}

void setar_termos_independentes_sl(SistemaLinear *sl, double *b) {
    copiar_vetor_double(sl->b, b, sl->n);
}

void destruir_config_gauss_seidel(ConfigGaussSeidel *s) {
    destruir_vetor(s->delta);
    destruir_vetor(s->X_old);
    free(s);
}
