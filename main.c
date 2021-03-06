/*
Vinícius Maurício Ribeiro GRR20203911
Gustavo Silveira Frehse GRR20203927
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "interface.h"
#include "criticante.h"

int main(int argc, char **argv) {
    int n;
    char * f_str;
    int max_iters;
    double epsilon;
    double *X0;
    
    FILE *outfile = stdout;
    
    int option;
    
    while ((option = getopt(argc, argv, "o:")) != -1) {
        if (option == 'o') {
            outfile = fopen(optarg, "w");
            
            if (!outfile) { 
                fprintf(stderr, "Nao foi possivel abrir o arquivo '%s' para escrita", optarg);
                return -1;
            }
        }
    }
    
    Criticante *crit_padrao;
    Criticante *crit_modificado;
    Criticante *crit_inexato;
    IterInfo info_padrao;
    IterInfo info_modificado;
    IterInfo info_inexato;
    
    
    while (!ler_dados_criticantes(&n, &f_str, &max_iters, &epsilon, &X0)) {
        crit_padrao = criar_criticante(f_str, max_iters, epsilon, X0, CRITICANTE_PADRAO);
        crit_modificado = criar_criticante(f_str, max_iters, epsilon, X0, CRITICANTE_MODIFICADO);
        crit_inexato = criar_criticante(f_str, max_iters, epsilon, X0, CRITICANTE_INEXATO);
        
        escrever_cabecalho(outfile, n, f_str);
             
        while (true) {  
            info_padrao = iterar_criticante(crit_padrao);
            info_modificado = iterar_criticante(crit_modificado);
            info_inexato = iterar_criticante(crit_inexato);
            
            if (acabou_criticante(crit_padrao) &&
                acabou_criticante(crit_modificado) &&
                acabou_criticante(crit_inexato)) {
                break;
            }

            escrever_dados(outfile, info_padrao, info_modificado, info_inexato);
       }

        escrever_tempos(outfile, info_padrao, info_modificado, info_inexato);

        free(X0);
        free(f_str);

        destruir_criticante(crit_padrao);
        destruir_criticante(crit_modificado);
        destruir_criticante(crit_inexato);
    }
    
    return 0;
}