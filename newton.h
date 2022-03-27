#ifndef NEWTON_H
#define NEWTON_H

#include <stdbool.h>
#include <stddef.h>

typedef struct iteracao {
    int i;
    int n;
    double tempo_total;
    double tempo_derivadas;
    double tempo_SL;
    char * f_str;
    char ** nomes_vars;
    void * f_evaluator;
    void * * * hessiana;
    void * * gradiente;
    double * * L;
    double * * U;
    double * X;
    int hess_steps;
    double epsilon;
    int max_iteracoes;
    bool acabou;
} iteracao;

iteracao* criar_iteracao(char* f_str, int n, double* chute_inicial, double epsilon, double max_iteracoes);

iteracao* iterar_newton_padrao(iteracao* iter);

iteracao* iterar_newton_modificado(iteracao* iter);

iteracao* iterar_newton_inexato(iteracao* iter);

void destroi_iteracao(iteracao* iter);

// calcula_hessiana(..);
// calcula_gradiente(..);
// while (i < maxIter) {
//     iterar_newton_padrao();
//     iterar_newton_modificado();
//     iterar_newton_inexato();
// }

#endif // NEWTON_H