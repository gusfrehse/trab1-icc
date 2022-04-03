#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>

#include "criticante.h"

int ler_dados_criticantes(int *n, char **f_str, int *max_iters, double *epsilon, double **X0);
int escrever_cabecalho(FILE *f, int n, char *f_str);
int escrever_dados(FILE *f, IterInfo padrao, IterInfo modificado, IterInfo inexato);
int escrever_tempos(FILE *f, IterInfo padrao, IterInfo modificado, IterInfo inexato);

#endif
