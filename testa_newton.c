#include "newton.h"
#include <stdio.h>
#include <stdlib.h>
#include <matheval.h>
#include <assert.h>

#define debug_print(fmt, var) \
    printf("%s:%d " #var " = " fmt "\n", __FILE__, __LINE__, (var))

#define FUNCAO_TESTE "9*x1-4*log(x1-7)"

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

        debug_print("%g", iter->tempo_SL);
        debug_print("%g", iter->tempo_derivadas);
        debug_print("%g", iter->tempo_total);
    }
}

int testa_newton_inexato_sla() {
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

        iterar_newton_inexato(iter);

        debug_print("%g", iter->tempo_SL);
        debug_print("%g", iter->tempo_derivadas);
        debug_print("%g", iter->tempo_total);
    }
}

int ler_funcao(iteracao** iter_a, iteracao** iter_b, iteracao** iter_c) {
    char * f_str;
    int num_vars;

    if (fscanf(stdin, "%d", &num_vars) == EOF) {
        return 0;
    }
    fscanf(stdin, "%ms", &f_str);

    double *chute = calloc(num_vars, sizeof(double));
    for (int i = 0; i < num_vars; i++) {
        fscanf(stdin, "%lg ", &chute[i]);
    }
    
    double epsilon;
    fscanf(stdin, "%lg", &epsilon);
    
    int max_iter;
    fscanf(stdin, "%d", &max_iter);
    
    *iter_a = criar_iteracao(f_str, num_vars, chute, epsilon, max_iter);
    *iter_b = criar_iteracao(f_str, num_vars, chute, epsilon, max_iter);
    *iter_c = criar_iteracao(f_str, num_vars, chute, epsilon, max_iter);
    
    return 1;
}

void testa_tres_newton() {
    double chute[] = {-3, -3, -3, -3, -3, -3, -3, -3, -3, -3};
    int num_vars = 10;
    iteracao *iter = criar_iteracao(FUNCAO_TESTE, num_vars, chute, 0.0000001, 40);
    
    
}

void print_tabela_comeco(iteracao* padrao, iteracao* modificado, iteracao* inexato) {
    // cabecalho    
    printf("Iteração \t| Newton Padrão \t| Newton Modificado \t| Newton Inexato\n");
}

void print_linha_tabelhinha(iteracao* padrao, iteracao* modificado, iteracao* inexato) {
    
    double f_X_padrao = evaluator_evaluate(padrao->f_evaluator, padrao->n, padrao->nomes_vars, padrao->X);
    double f_X_modificado = evaluator_evaluate(modificado->f_evaluator, modificado->n, modificado->nomes_vars, modificado->X);
    double f_X_inexato = evaluator_evaluate(inexato->f_evaluator, inexato->n, inexato->nomes_vars, inexato->X);
    
    
    printf("%d \t\t| ", padrao->i); // imprime iteração

    if(!padrao->acabou) {
        printf("%1.14e\t| ", f_X_padrao);
    }
    else {
        printf("\t\t\t| ");
    }

    if(!modificado->acabou) {
        printf("%1.14e\t| ", f_X_modificado);
    }
    else {
        printf("\t\t\t| ");
    }

    if(!inexato->acabou) {
        printf("%1.14e\t| ", f_X_inexato);
    }
    else {
        printf("\t\t\t| ");
    }
    
    printf("\n");
}

void print_tabela_fim(iteracao* padrao, iteracao* modificado, iteracao* inexato) {
    // footer
    printf("Tempo total \t| %1.14e\t| %1.14e\t| %1.14e\n", padrao->tempo_derivadas, modificado->tempo_total, inexato->tempo_total);
    printf("Tempo derivadas | %1.14e\t| %1.14e\t| %1.14e\n", padrao->tempo_derivadas, modificado->tempo_derivadas, inexato->tempo_derivadas);
    printf("Tempo SL \t| %1.14e\t| %1.14e\t| %1.14e\n", padrao->tempo_SL, modificado->tempo_SL, inexato->tempo_SL);
}

int main(void)
{
    double chute[] = { 7.01 };
    int num_vars = 1;
    int max_iters = 20;

    iteracao *padrao;
    iteracao *modificado;
    iteracao *inexato;
    
    while (ler_funcao(&padrao, &modificado, &inexato)) {
        print_tabela_comeco(padrao, modificado, inexato);
        
        for (int i = 0; i < padrao->max_iteracoes; i++)
        {
            print_linha_tabelhinha(padrao, modificado, inexato);

            iterar_newton_padrao(padrao);
            iterar_newton_modificado(modificado);
            iterar_newton_inexato(inexato);
        }
        print_linha_tabelhinha(padrao, modificado, inexato);

        
        print_tabela_fim(padrao, modificado, inexato);

        destroi_iteracao(padrao);
        destroi_iteracao(modificado);
        destroi_iteracao(inexato);
    }
    // testa_criar_iteracao();
    // testa_newton_padrao_sla();
    // testa_newton_modificado_sla();
    //testa_newton_inexato_sla();
}
