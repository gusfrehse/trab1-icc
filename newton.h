#ifndef NEWTON_H
#define NEWTON_H

typedef struct iteracao {
    int i;
    int n;
    double tempo_total;
    double tempo_derivadas;
    double tempo_SL;
    char * funcao_A;
} iteracao;

iteracao* criar_iteracao();

iteracao* iterar_newton_padrao(iteracao* iter);

iteracao* iterar_newton_modificado(iteracao* iter);

iteracao* iterar_newton_inexato(iteracao* iter);

#endif // NEWTON_H