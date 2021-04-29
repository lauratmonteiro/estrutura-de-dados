#include <limits.h>
#include <stdlib.h>

#include "cliente.c"

void imprime_arquivo(char *nome_arquivo_dados) {
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

int hash(int cod_cli) {
    return cod_cli % 7;
}

void excluiHashk(char *nome_arquivo, int k) {
    FILE *arq = fopen(nome_arquivo, "r+b");

    int prox = k;
    int h;

    fseek(arq, k*sizeof(TCliente), SEEK_SET);

    while(1){
        TCliente *cli = le_cliente(arq);

        if(cli->cod == -1){
            free(cli);
            break;
        }

        h = hash(cli->cod);

        imprime_cliente(cli);
        if(h == k){
            cli->ocupado = 0;

            fseek(arq, prox*sizeof(TCliente), SEEK_SET);
            salva_cliente(cli, arq);
        }
        prox = cli->prox;

        free(cli);
        if(prox == -1){
            break;
        }

        fseek(arq, prox*sizeof(TCliente), SEEK_SET);
    }

    fclose(arq);

}

int main() {
    /* Essa função gera a saída que é usada no teste do run.codes. Ela NÃO DEVE SER MODIFICADA */
    int k;

    //Imprime arquivos de entrada
    //imprime_arquivo("tabHash.dat");

    //le o valor de k
    scanf("%d", &k);
    printf("REGISTROS EXAMINADOS:\n");
    excluiHashk("tabHash.dat", k);
    printf("ARQUIVO FINAL:\n");
    imprime_arquivo("tabHash.dat");
}