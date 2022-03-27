#include "newton.h"
#include <stdio.h>
#include <matheval.h>
#include <assert.h>

#define debug_print(fmt, var) \
    printf("%s:%d " #var " = " fmt "\n", __FILE__, __LINE__, (var))

#define FUNCAO_TESTE "0.5*(x1^4-16*x1^2+5*x1+x2^4-16*x2^2+5*x2+x3^4-16*x3^2+5*x3+x4^4-16*x4^2+5*x4+x5^4-16*x5^2+5*x5+x6^4-16*x6^2+5*x6+x7^4-16*x7^2+5*x7+x8^4-16*x8^2+5*x8+x9^4-16*x9^2+5*x9+x10^4-16*x10^2+5*x10+0)"

void testa_criar_iteracao()
{
    double chute[] = {123123123};
    iteracao *iter = criar_iteracao(FUNCAO_TESTE, 1, chute, 0.1, 40);
    debug_print("%d", iter->n);
    debug_print("%s", iter->f_str);

    printf("Variaveis\n");
    for (int i = 0; i < iter->n; i++)
    {
        debug_print("variavel %s", iter->nomes_vars[i]);
    }

    // char* aff = ;
    debug_print("%s", evaluator_get_string(iter->f_evaluator));
    // debug_print("%lf", iter->);

    printf("Hessiana\n");
    for (int i = 0; i < iter->n; i++)
    {
        for (int j = 0; j < iter->n; j++)
        {
            char *val_str = evaluator_get_string(iter->hessiana[i][j]);
            printf("linha %d, col %d: %s \n", i, j, evaluator_get_string(iter->hessiana[i][j]));
        }
        printf("\n");
    }

    printf("grad\n");
    for (int i = 0; i < iter->n; i++)
    {
        printf("linha %d: %s \n", i, evaluator_get_string(iter->gradiente[i]));
    }
}

void testa_newton_padrao_sla()
{
    double chute[] = {0.1};
    int num_vars = 1;
    iteracao *iter = criar_iteracao(FUNCAO_TESTE, num_vars, chute, 0.0001, 40);

    while (true)
    {
        printf("%d\n", iter->i);
        // debug_print("%lg", evaluator_evaluate(iter->f_evaluator, num_vars, iter->nomes_vars, iter->X));
        debug_print("%d", iter->n);

        for (int i = 0; i < iter->n; i++)
        {
            printf("X[%d] 8===D %g \n", i, iter->X[i]);
        }

        getchar();

        iterar_newton_padrao(iter);
    }
}

void testa_newton_modificado_sla()
{
    double chute[] = {-3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
    int num_vars = 10;
    iteracao *iter = criar_iteracao(FUNCAO_TESTE, num_vars, chute, 0.0000001, 40);

    while (true)
    {
        printf("%d\n", iter->i);
        debug_print("%lg", evaluator_evaluate(iter->f_evaluator, num_vars, iter->nomes_vars, iter->X));

        for (int i = 0; i < iter->n; i++)
        {
            printf("X[%d] 8===D %g \n", i, iter->X[i]);
        }

        getchar();

        iterar_newton_modificado(iter);
    }
}

int main(void)
{
    // testa_criar_iteracao();
    // testa_newton_padrao_sla();
    testa_newton_modificado_sla();
}