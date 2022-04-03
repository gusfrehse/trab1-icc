#ifndef SISTEMA_H
#define SISTEMA_H

#define MAX_ITERS_GAUSS_SEIDEL 50

typedef struct SistemaLinear SistemaLinear;

typedef struct ConfigLU ConfigLU;
typedef struct ConfigGaussSeidel ConfigGaussSeidel;

SistemaLinear* alocar_sl(int n);

void criar_sl(SistemaLinear *sl, double** M, double *b);

void destruir_sl(SistemaLinear *sl);

void resolver_sl_eliminacao_gauss(SistemaLinear *sl);

void resolver_sl_LU(SistemaLinear *sl, ConfigLU *s);

void resolver_sl_gauss_seidel(SistemaLinear *sl, ConfigGaussSeidel *s);

ConfigLU *alocar_config_LU(SistemaLinear *sl);

void criar_config_LU(ConfigLU *config, SistemaLinear *sl);

void criar_config_gauss_seidel(ConfigGaussSeidel *config, SistemaLinear *sl);

ConfigGaussSeidel *alocar_config_gauss_seidel(SistemaLinear *sl);

void criar_config_gauss_seidel(ConfigGaussSeidel *config, SistemaLinear *sl);

void destruir_config_LU(ConfigLU *s);

void destruir_config_gauss_seidel(ConfigGaussSeidel *s);

double *solucao_sl(SistemaLinear *sl);

#endif
