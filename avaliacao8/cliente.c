#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_NOME 100

typedef struct Cliente {
    int cod;
    char nome[TAM_NOME];
    int prox;
    int ocupado; //Valores possíveis são 0 (LIBERADO) e 1 (OCUPADO)
} TCliente;

void imprime_cliente(TCliente *cliente)
{
    if (cliente != NULL) {
        printf("%3d, %12s, %3d, %8s\n", cliente->cod, cliente->nome, cliente->prox,
               (cliente->ocupado ? "OCUPADO" : "LIBERADO"));
    }
}

TCliente *cliente(int cod, char *nome, int prox, int flag)
{
	TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
	if (cliente) memset(cliente, 0, sizeof(TCliente));
	cliente->cod = cod;
	strcpy(cliente->nome, nome);
	cliente->prox = prox;
	cliente->ocupado = flag;
	return cliente;
}

void salva_cliente(TCliente *cliente, FILE *out)
{
	fwrite(&cliente->cod, sizeof(int), 1, out);
	fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
	fwrite(&cliente->prox, sizeof(int), 1, out);
	fwrite(&cliente->ocupado, sizeof(int), 1, out);
}

TCliente *le_cliente(FILE *in)
{
	TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
	if (0 >= fread(&cliente->cod, sizeof(int), 1, in)) {
		free(cliente);
		return NULL;
	}
	fread(cliente->nome, sizeof(char), sizeof(cliente->nome), in);
	fread(&cliente->prox, sizeof(int), 1, in);
	fread(&cliente->ocupado, sizeof(int), 1, in);
	return cliente;
}

int tamanho_cliente()
{
	return sizeof(int) + // cod_cliente
		sizeof(char) * TAM_NOME + // nome
		sizeof(int) + // prox
		sizeof(int); //flag
}

void imprime_arquivo_dados(char *nome_arquivo_dados) {
    FILE *in = fopen(nome_arquivo_dados, "rb");
    TCliente *cliente = le_cliente(in);
    int pos = 0;
    while (cliente != NULL) {
        printf("[%3d] %3d, %12s, %3d, %8s\n", pos, cliente->cod, cliente->nome, cliente->prox,
               (cliente->ocupado? "OCUPADO" : "LIBERADO"));
        pos++;
        cliente = le_cliente(in);
    }
    fclose(in);
}

int num_registros(char *nome_arquivo_dados) {
    // calcula numero de registros armazenados no arquivo
    int num_registros = 0;
    FILE *dados = fopen(nome_arquivo_dados, "rb");
    fseek(dados, 0, SEEK_END);
    num_registros = ftell(dados) / tamanho_cliente();
    fclose(dados);
    return num_registros;
}
