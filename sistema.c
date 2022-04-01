#include "sistema.h"

typedef struct SistemaLinear {
    int n;
    double *b, *X;

    union {
        struct {
            double **L, **U;
        };
        
        double **M;
    };
} SistemaLinear;

typedef struct ConfigLU {
    int *trocas;
} ConfigLU;

typedef struct ConfigGaussSeidel {
    double *X_old, *delta;
} ConfigGaussSeidel;

