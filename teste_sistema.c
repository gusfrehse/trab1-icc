#include <stdio.h>

#include "sistema.h" 
#include "utilidades.h"

#define N 3

int main(void) {
    double **m = (double **) criar_matriz(sizeof(double), N);
    double *b = (double *) criar_vetor(sizeof(double), N);
    
    // x = 3; y = 4
    // x + y =  7
    // x - y = -1
    m[0][0] = 1;
    m[0][1] = 3;
    m[0][2] = -1;
    b[0] = 0;

    m[1][0] =  2;
    m[1][1] =  1;
    m[1][2] =  1;
    b[1] =  1;
    
    m[2][0] = 3;
    m[2][1] = -1;
    m[2][2] =  1;
    b[2] =  3;

    SistemaLinear *sl = alocar_sl(N);
    criar_sl(sl, m, b);
    
    //ConfigLU *config_lu = criar_config_LU(sl);
    ConfigGaussSeidel *config_gs = criar_config_gauss_seidel(sl);
    
    //resolver_sl_eliminacao_gauss(sl);
    //resolver_sl_LU(sl, config_lu);
    resolver_sl_gauss_seidel(sl, config_gs);
    double *x = solucao_sl(sl);
    for (int i = 0; i < N; i++) {
        printf("%g ", x[i]);
    }

    return 0;
}