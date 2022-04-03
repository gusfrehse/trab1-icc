#ifndef CRITICANTE_H
#define CRITICANTE_H

#include <stdbool.h>

typedef struct IterInfo {
    int iteracao;
    double tempo_total;
    double tempo_derivadas;
    double tempo_SL;
    double f_x;
    bool acabou;
} IterInfo;

typedef enum TipoCriticante {
    CRITICANTE_PADRAO,
    CRITICANTE_MODIFICADO,
    CRITICANTE_INEXATO
} TipoCriticante;

typedef struct Criticante Criticante;

Criticante *criar_criticante(char* f_str, int max_iters, double epsilon, double *X0, TipoCriticante tipo);

IterInfo iterar_criticante(Criticante *c);

bool acabou_criticante(Criticante *crit);

void destruir_criticante(Criticante *crit);

double *solucao_criticante(Criticante* c);

#endif
