#ifndef SISTEMA_H
#define SISTEMA_H


typedef struct SistemaLinear SistemaLinear;

typedef struct ConfigLU ConfigLU;
typedef struct ConfigGaussSeidel ConfigGaussSeidel;

SistemaLinear* alocar_sl(int n);

void criar_sl(SistemaLinear *sl, double** M, double *b);

void destruir_sl(SistemaLinear *sl);

void resolver_sl_eliminacao_gauss(SistemaLinear *sl);

void resolver_sl_LU(SistemaLinear *sl, ConfigLU *s);

void resolver_sl_gauss_seidel(SistemaLinear *sl, ConfigGaussSeidel *s);

ConfigLU *criar_config_LU(SistemaLinear *sl);

ConfigGaussSeidel *criar_config_gauss_seidel(SistemaLinear *sl);

#endif