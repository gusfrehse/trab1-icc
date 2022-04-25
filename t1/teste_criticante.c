#include <stdio.h>

#include "utilidades.h"
#include "criticante.h"

#define FUNCAO "-log(1-x1-x2)-log(x1)-log(x2)"
#define EPS 0.0000001
#define MAX_ITERS 100000
#define X0_VET {0.85, 0.05}
#define TIPO CRITICANTE_INEXATO

int main(void) {

    double X0[] = X0_VET;
    
    Criticante *c1 = criar_criticante(FUNCAO, MAX_ITERS, EPS, X0, CRITICANTE_PADRAO); 
    Criticante *c2 = criar_criticante(FUNCAO, MAX_ITERS, EPS, X0, CRITICANTE_MODIFICADO); 
    Criticante *c3 = criar_criticante(FUNCAO, MAX_ITERS, EPS, X0, CRITICANTE_INEXATO); 

    IterInfo i1;
    IterInfo i2;
    IterInfo i3;

    while(true) {
        i1 = iterar_criticante(c1);
        i2 = iterar_criticante(c2);
        i3 = iterar_criticante(c3);

        printf("f(X): %g; i: %d; t deriv: %g; t SL: %g; t Total: %g\n", i1.f_x, i1.iteracao, i1.tempo_derivadas, i1.tempo_SL, i1.tempo_total);
        printf("f(X): %g; i: %d; t deriv: %g; t SL: %g; t Total: %g\n", i2.f_x, i2.iteracao, i2.tempo_derivadas, i2.tempo_SL, i2.tempo_total);
        printf("f(X): %g; i: %d; t deriv: %g; t SL: %g; t Total: %g\n", i3.f_x, i3.iteracao, i3.tempo_derivadas, i3.tempo_SL, i3.tempo_total);
        print_vetor_double(solucao_criticante(c1), 2);
        print_vetor_double(solucao_criticante(c2), 2);
        print_vetor_double(solucao_criticante(c3), 2);
        
        printf("\n");
        getchar();
    }
}
