#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "criticante.h"

/**
 * @brief Escreve dados de uma iteração.
 * 
 * @param f Arquivo de saída.
 * @param i Informação de uma iteração.
 */
static void escrever_dados_iteracao(FILE *f, IterInfo i);

int ler_dados_criticantes(int *n, char **f_str, int *max_iters, double *epsilon, double **X0) {
    int lidos = 0;
    
    lidos = scanf("%d ", n);
    if (lidos != 1) {
        return -1;
    }
    lidos = scanf("%ms", f_str);
    if (lidos != 1) {
        return -1;
    }

    *X0 = (double *) calloc(*n, sizeof(double));
    if (!*X0) {
        free(f_str);
        return -1;   
    }

    for (int i = 0; i < *n; i++) {
        lidos = scanf("%lg ", &(*X0)[i]);
        if (lidos != 1) {
            free(f_str);
            free(*X0);
            return -1;
        }
    }

    lidos = scanf("%lg", epsilon);
    if (lidos != 1) {
        free(f_str);
        free(*X0);
        return -1;
    }
    lidos = scanf("%d", max_iters);
    if (lidos != 1) {
        free(f_str);
        free(*X0);
        return -1;
    }
    
    return 0;
}

int escrever_cabecalho(FILE *f, int n, char *f_str) {
    fprintf(f, "%d\n", n);
    fprintf(f, "%s\n", f_str);
    fprintf(f, "#\n");
    fprintf(f, "Iteração \t| Newton Padrão \t| Newton Modificado \t| Newton Inexato\t|\n");


    return 0;
}

static void escrever_dados_iteracao(FILE *f, IterInfo i) {
    if (i.ocorreu_erro) {
        fprintf(f, "ERRO\t\t\t| ");
        return;
    }
   
    if(!i.acabou) {
        
        if(isnan(i.f_x) || isinf(i.f_x))
            fprintf(f, "%1.14e\t\t\t| ", i.f_x);
        else
            fprintf(f, "%1.14e\t| ", i.f_x);
    }
    else {
        fprintf(f, "\t\t\t| ");
    }
}

int escrever_dados(FILE *f, IterInfo padrao, IterInfo modificado, IterInfo inexato) {
    
    fprintf(f, "%d \t\t| ", padrao.iteracao);
    
    escrever_dados_iteracao(f, padrao);
    escrever_dados_iteracao(f, modificado);
    escrever_dados_iteracao(f, inexato);

    fprintf(f, "\n");

    return 0;
}

int escrever_tempos(FILE *f, IterInfo padrao, IterInfo modificado, IterInfo inexato) {

    fprintf(f, "Tempo total \t| %1.14e\t| %1.14e\t| %1.14e\t|\n",
            padrao.tempo_total, modificado.tempo_total, inexato.tempo_total);

    fprintf(f, "Tempo derivadas | %1.14e\t| %1.14e\t| %1.14e\t|\n",
            padrao.tempo_derivadas, modificado.tempo_derivadas, inexato.tempo_derivadas);

    fprintf(f, "Tempo SL \t| %1.14e\t| %1.14e\t| %1.14e\t|\n",
            padrao.tempo_SL, modificado.tempo_SL, inexato.tempo_SL);

    fprintf(f, "#\n");

    return 0;
}
