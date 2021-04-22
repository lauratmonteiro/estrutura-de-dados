#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "no_folha.c"

typedef struct Metadados {
    int d; // ordem da arvore
    // ponteiro para a raiz da arvore
    int pont_raiz;
    // flag que diz se a raiz eh uma folha (nesse caso ela esta no arquivo de dados)
    int raiz_folha;
    // ponteiro para o proximo noh interno livre
    int pont_prox_no_interno_livre;
    // ponteiro para o proximo noh folha livre
    int pont_prox_no_folha_livre;
} TMetadados;

void imprime_metadados(TMetadados *metadados)
{
    printf("d = %d, pt raiz = %d, raiz folha = %d\npt prox no interno livre = %d, pt prox no folha livre = %d\n", metadados->d, metadados->pont_raiz, metadados->raiz_folha,
           metadados->pont_prox_no_interno_livre, metadados->pont_prox_no_folha_livre);
}

TMetadados *metadados(int d, int pont_raiz, int raiz_folha, int pont_prox_no_interno_livre, int pont_prox_no_folha_livre)
{
	TMetadados *metadados = (TMetadados *) malloc(sizeof(TMetadados));
	if (metadados) memset(metadados, 0, sizeof(TMetadados));
	metadados->d = d;
	metadados->pont_raiz = pont_raiz;
	metadados->raiz_folha = raiz_folha;
	metadados->pont_prox_no_interno_livre = pont_prox_no_interno_livre;
	metadados->pont_prox_no_folha_livre = pont_prox_no_folha_livre;
	return metadados;
}

TMetadados *metadados_vazio(int d)
{
	return metadados(d, 0, 1, 0, tamanho_no_folha(d));
}

void salva_metadados(TMetadados *metadados, FILE *out)
{
	fwrite(&metadados->d, sizeof(int), 1, out);
	fwrite(&metadados->pont_raiz, sizeof(int), 1, out);
	fwrite(&metadados->raiz_folha, sizeof(int), 1, out);
	fwrite(&metadados->pont_prox_no_interno_livre, sizeof(int), 1, out);
	fwrite(&metadados->pont_prox_no_folha_livre, sizeof(int), 1, out);
}

void salva_arq_metadados(char *nome_arquivo, TMetadados *metadados)
{
	FILE *out = fopen(nome_arquivo, "wb");
	salva_metadados(metadados, out);
	fclose(out);
}


TMetadados *le_metadados(FILE *in)
{
	TMetadados *metadados = (TMetadados *) malloc(sizeof(TMetadados));
	if (0 >= fread(&metadados->d, sizeof(int), 1, in)) {
		free(metadados);
		return NULL;
	}
	fread(&metadados->pont_raiz, sizeof(int), 1, in);
	fread(&metadados->raiz_folha, sizeof(int), 1, in);
	fread(&metadados->pont_prox_no_interno_livre, sizeof(int), 1, in);
	fread(&metadados->pont_prox_no_folha_livre, sizeof(int), 1, in);
	return metadados;
}

TMetadados *le_arq_metadados(char *nome_arquivo)
{
	FILE *in = fopen(nome_arquivo, "rb");
	TMetadados *m = le_metadados(in);
	fclose(in);
	return m;
}

int tamanho_metadados()
{
	return sizeof(int) + //d
	    sizeof(int) + // pont_raiz
		sizeof(int) + // raiz_folha
		sizeof(int) + // pont_prox_no_interno_livre
		sizeof(int); // pont_prox_no_folha_livre
}