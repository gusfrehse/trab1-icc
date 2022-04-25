struct SistemaLinear_1 {
    int n;
    double *b, *X;

    union {
        struct {
            double **M;
        } padrao;

        struct {
            double **L, **U;
            int *trocas;
        } modificado;

        struct {
            double **M;
            double *X_old, *delta;
        } inexato;
    };
};

struct SistemaLinear_2 {
    int n;
    double *b, *X;

    union {
        struct {
            double **L, **U;
        };
        
        double **M;
    };
};
struct EstadoSeidel_2 {
    double *X_old, *delta;
};
struct EstadoLU_2 {
    int *trocas;
};