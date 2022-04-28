# Otimizações
## Mudanca no laço interno do `retrossubs()`
Antes o compilador não conseguiu usar simd, depois conseguiu.

Antes:
	for (int j = i + 1; j < n; j++) {
		X[i] -= M[i][j] * X[j];
	}

Depois:
	double sub = 0.0;
	for (int j = i + 1; j < n; j++) {
		sub += M[i][j] * X[j];
	}
	X[i] -= sub;
    
## Mudanca no laço interno do `resolver_sistema_gausseidel()`
Antes o compilador não conseguiu usar simd, depois conseguiu.

Antes:
	for (int j = 0; j < i; j++) {
		X[i] -= sl->M[i][j] * sl->X[j];
	}

	for (int j = i + 1; j < sl->n; j++) {
		X[i] -= sl->M[i][j] * sl->X[j];
	}

Depois:
	double soma = 0.0;
	for (int j = 0; j < i; j++) {
		soma += sl->M[i][j] * sl->X[j];
	}

	for (int j = i + 1; j < sl->n; j++) {
		soma += sl->M[i][j] * sl->X[j];
	}
	sl->X[i] -= soma;
