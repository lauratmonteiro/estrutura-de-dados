#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Nomes {
    char *nome;
    struct Nomes *prox;
} TNomes;

void imprime_nomes(TNomes *nomes)
{
    while(nomes) {
        printf("%s\n", nomes->nome);
        nomes = nomes->prox;
    }
}

TNomes* insere_inicio (TNomes* li, char *nome) {
    TNomes* novo = (TNomes*) malloc(sizeof(TNomes));
    novo->nome = nome;
    novo->prox = li;
    li = novo;
    return novo;
}

TNomes* insere_fim (TNomes* li, char *nome) {
    TNomes* novo = (TNomes*) malloc(sizeof(TNomes));
    novo->nome = nome;
    novo->prox = NULL;
    TNomes* p = li;
    TNomes* q = li;
    while (p != NULL) {
        /* encontra o ultimo elemento */
        q = p;
        p = p->prox;
    }
    if (q != NULL) /* se a lista original não estiver vazia */ 
        q->prox = novo;
    else
        li = novo;
    return li;
}


void libera_nomes(TNomes *nomes)
{
	TNomes *temp = NULL;
	while(nomes) {
		temp = nomes->prox;
		free(nomes->nome);
		free(nomes);
		nomes = temp;
	}
}

int conta_nomes(TNomes *nomes)
{
	int qtd = 0;
	while(nomes) {
		qtd += 1;
		nomes = nomes->prox;
	}
	return qtd;
}

