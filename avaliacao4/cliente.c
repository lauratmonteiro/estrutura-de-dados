#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_NOME 100

typedef struct Cliente {
    int cod_cliente;
    char nome[TAM_NOME];
} TCliente;

void imprime_cliente(TCliente *cliente)
{
	printf("%d, %s\n", cliente->cod_cliente, cliente->nome);
}

TCliente *cliente(int cod, char *nome)
{
	TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
	if (cliente) memset(cliente, 0, sizeof(TCliente));
	cliente->cod_cliente = cod;
	strcpy(cliente->nome, nome);
	return cliente;
}

void salva_cliente(TCliente *cliente, FILE *out)
{
	fprintf(out, "%d", cliente->cod_cliente);
	fprintf(out, "%c", ';');
	fprintf(out, "%s", cliente->nome);
	fprintf(out, "%c", ';');
	fprintf(out, "%s", "\n");
}

TCliente *le_cliente(FILE *in)
{
	TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    char linha[150];
    if (fgets (linha, 150, in) == NULL ) {
        free(cliente);
        return NULL;
    }

    char delimitador[] = ";";
    char *ptr;
    int cod;

    //quebra a linha
    ptr = strtok(linha, delimitador);
    cod = atoi(ptr);
    cliente->cod_cliente = cod;
    ptr = strtok(NULL, delimitador);
    strcpy(cliente->nome, ptr);

	return cliente;
}