#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cliente.c"

typedef struct NoFolha {
    // m eh o numero total de registros armazenados no noh
    int m;
    // ponteiro para o noh pai
    int pont_pai;
    // ponteiro para o proximo noh folha
    int pont_prox;
    // essa lista tera sempre m clients
    TCliente **clientes;
} TNoFolha;

/*
 * A função imprime um nó folha
 * na primeira linha são impressos os metadados
 * depois são impressos os registros de clientes
 */
void imprime_no_folha(int d, TNoFolha *no)
{
	int i;
	printf("NO FOLHA: ");
	printf("(m = %d, pt pai = %d, pt prox = %d)\n", no->m, no->pont_pai, no->pont_prox);
	for (i = 0; i < 2 * d; i++) {
		printf("\t");
		if (no->clientes[i])
			imprime_cliente(no->clientes[i]);
	}
	printf("\n");
}

TNoFolha *no_folha(int d, int m, int pont_pai, int pont_prox)
{
	int i;
	TNoFolha *no = (TNoFolha *) malloc(sizeof(TNoFolha));
	if (no) memset(no, 0, sizeof(TNoFolha));
	no->m = m;
	no->pont_pai = pont_pai;
	no->pont_prox = pont_prox;
	no->clientes = (TCliente **) malloc(sizeof(TCliente *) * 2 * d);
	for (i = 0; i < 2 * d; i++) {
		no->clientes[i] = NULL;
	}
	return no;
}

TNoFolha *no_folha_vazio(int d)
{
	return no_folha(d, 0, -1, -1);
}

TNoFolha *cria_no_folha(int d, int pont_pai, int pont_prox, int m, ...)
{
	TNoFolha *n = no_folha(d, m, pont_pai, pont_prox);
	int i;
	va_list ap;
	va_start(ap, m);
	for (i = 0; i < n->m; i++) {
		n->clientes[i] = va_arg(ap, TCliente *);
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

void salva_no_folha(int d, TNoFolha *no, FILE *out)
{
	int i;
	fwrite(&no->m, sizeof(int), 1, out);
	fwrite(&no->pont_pai, sizeof(int), 1, out);
	fwrite(&no->pont_prox, sizeof(int), 1, out);
	//garantidamente, sempre havera pelo menos 1 chave no noh
    //portanto, p0 sempre vai existir
	TCliente *vazio = cliente(-1, "");

	for (i = 0; i < 2 * d; i++) {
		if (no->clientes[i]) {
			salva_cliente(no->clientes[i], out);
		} else {
			salva_cliente(vazio, out);
		}
	}
	free(vazio);
}

/*
 * d é a ordem da arvore
 * in é o arquivo que armazena os nós folha da árvore
 * Retorna um ponteiro para o nó em memória, ou NULL caso ocorra erro na leitura (ou o arquivo tenha chegado ao fim)
 */

TNoFolha *le_no_folha(int d, FILE *in)
{
	int i;
	TNoFolha *no = (TNoFolha *) malloc(sizeof(TNoFolha));
	if (0 >= fread(&no->m, sizeof(int), 1, in)) {
		free(no);
		return NULL;
	}
	fread(&no->pont_pai, sizeof(int), 1, in);
	fread(&no->pont_prox, sizeof(int), 1, in);
	no->clientes = (TCliente **) malloc(sizeof(TCliente *) * 2 * d);

	for (i = 0; i < no->m; i++) {
		no->clientes[i] = le_cliente(in);
	}

	// Termina de ler dados nulos para resolver problema do cursor
	// Dados lidos sao descartados
	TCliente *vazio;
	for (i = no->m; i < 2 * d; i++) {
		no->clientes[i] = NULL;
		vazio = le_cliente(in);
		free(vazio);
	}
	return no;
}

int tamanho_no_folha(int d)
{
	return sizeof(int) + // m
		sizeof(int) + // pont_pai
		sizeof(int) + // pont_prox
		tamanho_cliente() * (2 * d); // clientes
}

void libera_no_folha(int d, TNoFolha *no)
{
	int i;
	for (i = 0; i < 2 * d; i++) {
		free(no->clientes[i]);
	}
	free(no->clientes);
	no->clientes = NULL;
	free(no);
}