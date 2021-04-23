#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct CompartimentoHash {
    int prox;
} TCompartimento;


void imprime_compartimento(TCompartimento *compartimento)
{
	printf("%d\n", compartimento->prox);
}

// Cria compartimento hash. Lembrar de usar free(compartimento_hash)
TCompartimento *compartimento_hash(int prox)
{
	TCompartimento *compartimento = (TCompartimento *) malloc(sizeof(TCompartimento));
	compartimento->prox = prox;
	return compartimento;
}

// Salva compartimento no arquivo out, na posicao atual do cursor
void salva_compartimento(TCompartimento *compartimento, FILE *out)
{
	fwrite(&compartimento->prox, sizeof(int), 1, out);
}

// Le um compartimento do arquivo in na posicao atual do cursor
// Retorna um ponteiro para compartimento lido do arquivo
TCompartimento *le_compartimento(FILE *in)
{
	TCompartimento *compartimento = (TCompartimento *) malloc(sizeof(TCompartimento));
	if (0 >= fread(&compartimento->prox, sizeof(int), 1, in)) {
		free(compartimento);
		return NULL;
	}
	return compartimento;
}

int tamanho_compartimento()
{
	return sizeof(int); // prox
}

void imprime_arquivo_hash(char *nome_arquivo_hash) {
    FILE *in = fopen(nome_arquivo_hash, "rb");
    TCompartimento *c = le_compartimento(in);
    int pos = 0;
    while (c != NULL) {
        printf("[%d] %3d\n", pos, c->prox);
        pos ++;
        c = le_compartimento(in);
    }
    fclose(in);
}
