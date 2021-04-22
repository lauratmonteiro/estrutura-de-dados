#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct NoInterno {
    // m eh o numero total de chaves armazenados no noh
    int m;
    // ponteiro para o noh pai
    int pont_pai;
    // flag que diz se noh aponta pra folha ou nao
    int aponta_folha;
    // essa lista tera sempre m + 1 ponteiros
    int *p;
    // essa lista tera sempre m chaves
    int *chaves;
} TNoInterno;

/*
 * A função imprime um nó interno
 * na primeira linha são impressos os metadados
 * na segunda linha são impressos os valores dos ponteiros (entre colchetes), e os valores da chaves
 * sempre na ordem [pt0] chave1 [pt1] chave2 [pt3] ... [pt2d]
 * ponteiros para NULL são representados por -1
 * chaves não existentes são representadas por -1
 */
void imprime_no_interno(int d, TNoInterno *no)
{
	int i;
    printf("NO INTERNO: ");
	printf("(m = %d, pt pai = %d, aponta folha = %d)\n\t", no->m, no->pont_pai, no->aponta_folha);
	for (i = 0; i < 2 * d; i++) {
		printf("[%d] ", no->p[i]);
        printf(" %d ", no->chaves[i]);
	}
	printf(" [%d] \n\n", no->p[i]);
}

TNoInterno *no_interno(int d, int m, int pont_pai, int aponta_folha)
{
	int i;
	TNoInterno *no = (TNoInterno *) malloc(sizeof(TNoInterno));
	if (no) memset(no, 0, sizeof(TNoInterno));
	no->m = m;
	no->pont_pai = pont_pai;
	no->aponta_folha = aponta_folha;
	no->p = (int *) malloc(sizeof(int) * (2 * d + 1));
	no->chaves = (int *) malloc(sizeof(int) * (2 * d));
	for (i = 0; i < 2 * d; i++) {
		no->p[i] = -1;
		no->chaves[i] = -1;
	}
	no->p[2 * d] = -1;
	return no;
}

TNoInterno *no_interno_vazio(int d)
{
	return no_interno(d, 0, -1, 0);
}

TNoInterno *cria_no_interno(int d, int m, int pont_pai, int aponta_folha, int size, ...)
{
	TNoInterno *n = no_interno(d, m, pont_pai, aponta_folha);
	int i;
	va_list ap;
	va_start(ap, size);
	for (i = 0; i < n->m + 1; i++) {
		n->p[i] = va_arg(ap, int);
	}
	for (i = 0; i < n->m; i++) {
		n->chaves[i] = va_arg(ap, int);
	}
	va_end(ap);
	return n;
}

/*
 * d é a ordem da arvore
 * no é o nó que vai ser salvo
 * out é o arquivo que armazena os nós internos da árvore
 *
 * A função assume que o arquivo já está aberto e o cursor já está posicionado
 * no ponto onde o nó deve ser escrito
 */

void salva_no_interno(int d, TNoInterno *no, FILE *out)
{
	int i;
	fwrite(&no->m, sizeof(int), 1, out);
	fwrite(&no->pont_pai, sizeof(int), 1, out);
	fwrite(&no->aponta_folha, sizeof(int), 1, out);
	//garantidamente, sempre havera pelo menos 1 chave no noh
    //portanto, p0 sempre vai existir
	fwrite(&no->p[0], sizeof(int), 1, out);

	for (i = 0; i < 2 * d; i++) {
		fwrite(&no->chaves[i], sizeof(int), 1, out);
		fwrite(&no->p[i + 1], sizeof(int), 1, out);
	}
}

/*
 * d é a ordem da arvore
 * in é o arquivo que armazena os nós internos da árvore
 * Retorna um ponteiro para o nó em memória, ou NULL caso ocorra erro na leitura (ou o arquivo tenha chegado ao fim)
 */
TNoInterno *le_no_interno(int d, FILE *in)
{
	int i;
	TNoInterno *no = (TNoInterno *) malloc(sizeof(TNoInterno));
	if (0 >= fread(&no->m, sizeof(int), 1, in)) {
		free(no);
		return NULL;
	}
	fread(&no->pont_pai, sizeof(int), 1, in);
	fread(&no->aponta_folha, sizeof(int), 1, in);
	no->p = (int *) malloc(sizeof(int) * (2 * d + 1));
	no->chaves = (int *) malloc(sizeof(int) * (2 * d));

	fread(&no->p[0], sizeof(int), 1, in);
	for (i = 0; i < 2 * d; i++) {
		fread(&no->chaves[i], sizeof(int), 1, in);
		fread(&no->p[i + 1], sizeof(int), 1, in);
	}
	return no;
}

int tamanho_no_interno(int d)
{
	return sizeof(int) + // m
		sizeof(int) + // pont_pai
		sizeof(int) + // aponta_folha
		sizeof(int) * (2 * d + 1) + // p
		sizeof(int) * (2 * d); // chaves
}

void libera_no_interno(TNoInterno *no)
{
	free(no->p);
	free(no->chaves);
	free(no);
}