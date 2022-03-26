#ifndef SIM_H
#define SIM_H

double *resolver_sistema(double **M, double *b, int n);
double *resolver_sistema_LU(double **M, double *b, int n);
double *resolver_sistema_gauss_seidel(double **M, double *b, int n);

#endif