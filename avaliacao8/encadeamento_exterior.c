#include <limits.h>
#include <stdio.h>

#include "cliente.c"
#include "compartimento_hash.c"

#define M 7

void imprime_arquivos(char *nome_arquivo_hash, char *nome_arquivo_dados) {
    //Imprime arquivo de hash
    printf("TABELA HASH:\n");
    imprime_arquivo_hash(nome_arquivo_hash);

    //Imprime arquivo de dados
    printf("\nDADOS:\n");
    imprime_arquivo_dados(nome_arquivo_dados);
}

int hash(int cod_cli, int m){
    return cod_cli % m;
}

/* Executa insercao em Arquivos por Encadeamento Exterior (Hash)
 * cod_cli: chave do cliente que esta sendo inserido
 * nome_cli: nome do cliente a ser inserido
 * nome_arquivo_hash: nome do arquivo que contem a tabela hash
 * nome_arquivo_dados: nome do arquivo onde os dados estao armazenados
 * Retorna o endereco (lógico) onde o cliente foi inserido, ou -1 se nao conseguiu inserir
 */
int insere(int cod_cli, char *nome_cli, char *nome_arquivo_hash, char *nome_arquivo_dados, int m) {
    int h = hash(cod_cli, m);
    TCliente *c = cliente(cod_cli, nome_cli, -1, 1);

    FILE *arq_hash = fopen(nome_arquivo_hash, "r+b");
    FILE *arq_dados = fopen(nome_arquivo_dados, "r+b");

    fseek(arq_hash, h*sizeof(int), SEEK_SET);
    TCompartimento *comp = le_compartimento(arq_hash);

    int pos = -1;
    if(comp->prox == -1){
        fseek(arq_dados, 0, SEEK_END);

        pos = ftell(arq_dados)/sizeof(TCliente);

        salva_cliente(c, arq_dados);

        comp->prox = pos;

        fseek(arq_hash, h*sizeof(int), SEEK_SET);

        salva_compartimento(comp, arq_hash);
    }else{
        fseek(arq_dados, comp->prox*sizeof(TCliente), SEEK_SET);
        TCliente *cli = le_cliente(arq_dados);
        int atual = comp->prox;

        while(1){
            if(cod_cli == cli->cod) break;

            if(!cli->ocupado){
                fseek(arq_dados, atual*sizeof(TCliente), SEEK_SET);

                pos = atual;
                c->prox = cli->prox;
                salva_cliente(c, arq_dados);

                break;
            }
            if(cli->prox == -1){
                fseek(arq_dados, 0, SEEK_END);
                pos = ftell(arq_dados)/sizeof(TCliente);

                salva_cliente(c, arq_dados);
                cli->prox = pos;

                fseek(arq_dados, atual*sizeof(TCliente), SEEK_SET);
                salva_cliente(cli, arq_dados);

                break;
            }
            atual = cli->prox;

            TCliente *aux = cli;
            fseek(arq_dados, cli->prox*sizeof(TCliente), SEEK_SET);
            cli = le_cliente(arq_dados);
            free(aux);
        }
        free(cli);
    }


    fclose(arq_hash);
    fclose(arq_dados);
    free(c);
    free(comp);

    return pos;
}

int main() {
    /* Essa função gera a saída que é usada no teste do run.codes. Ela NÃO DEVE SER MODIFICADA */
    int cod;
    char nome[TAM_NOME] = "";
    int pont;

    //le dados a serem inseridos
    scanf("%d", &cod);
    scanf("%s", nome);

    //Imprime arquivos de entrada
    // imprime_arquivos("tabHash.dat", "clientes.dat");

    pont = insere(cod, nome, "tabHash.dat", "clientes.dat", M);
    imprime_arquivos("tabHash.dat", "clientes.dat");
    //Imprime resultado da função
    printf("\nRESULTADO: %d", pont);
}