#ifndef SIM_H
#define SIM_H

void calcula_LU(double **L, double **U, int *trocas, double **M, int n);

double *resolver_sistema(double **M, double *b, int n);
double *resolver_sistema_LU(double **L, double **U, double *b, int* trocas, int n);
double *resolver_sistema_gauss_seidel(double **M, double *b, int n);

#endif