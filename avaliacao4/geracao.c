#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "cliente.c"

#include <dirent.h>
#include <string.h>

typedef struct vetor {
    TCliente *cli;
    int congelado;
} TVet;

void imprime_arquivo(char *nome) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(nome, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets(linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets(linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

char *gera_nome_particao(int num_part) {
    char *nome_particao = (char *) malloc(sizeof(char[10]));
    char num_particao[10];

    strcpy(nome_particao, "p");
    sprintf(num_particao, "%d", num_part);
    strcat(nome_particao, num_particao);
    strcat(nome_particao, ".txt");

    return nome_particao;
}

int retorna_pos_menor(TVet* mem, int tam_memoria){
    int menor;
    int i;
    int pos = -1;
    for(i = 0; i<tam_memoria; i++){
        if(mem[i].cli != NULL && !mem[i].congelado){
            menor = mem[i].cli->cod_cliente; 
            pos = i;
            break;
        }
    }
    
    
    for(int j = i;j<tam_memoria; j++){
        if(mem[j].cli != NULL && mem[j].cli->cod_cliente < menor && !mem[j].congelado){
            menor = mem[j].cli->cod_cliente;
            pos = j;
        }
    }

    return pos;
}

int selecao_substituicao(char *nome_arquivo_entrada, int tam_memoria) {
    //TODO: Implementar essa função
    TVet *mem = (TVet*)malloc(sizeof(TVet)*tam_memoria);
    FILE *arq = fopen(nome_arquivo_entrada, "r");
    int usado = 0;


    if(arq == NULL){
        return 0;
    }

    for(int i = 0; i<tam_memoria; i++){
        if(feof(arq)) break;

        TCliente* cli = le_cliente(arq);

        TVet vet;
        vet.cli = cli;
        vet.congelado = 0;
        mem[i] = vet;
        usado++;
    }

    int p = 1;
    FILE* part = fopen(gera_nome_particao(p), "w");
    while(1){
        int pos_menor = retorna_pos_menor(mem, tam_memoria);
        if(pos_menor == -1){
            fclose(part);
            if (usado == 0) {
                break;
            }

            p+=1;
            FILE* part = fopen(gera_nome_particao(p), "w");
            for(int i = 0; i<tam_memoria; i++){
                if (mem[i].cli != NULL) mem[i].congelado = 0;
            }

            continue;
        }

        salva_cliente(mem[pos_menor].cli, part);
        mem[pos_menor].congelado = 1;

        int ant = mem[pos_menor].cli->cod_cliente;
        free(mem[pos_menor].cli);
        mem[pos_menor].cli = NULL;

        TCliente* cli = le_cliente(arq);
        if (cli != NULL) {
            TVet vet;
            vet.cli = cli;
            vet.congelado = 0;
            mem[pos_menor] = vet;


            if(mem[pos_menor].cli->cod_cliente < ant){
                mem[pos_menor].congelado = 1;
            }
        } else {
            usado--;
        }
        
    }
    return p;
}

int main() {
    int tam_memoria;
    scanf("%d", &tam_memoria);
    int num_part = selecao_substituicao("entrada.txt", tam_memoria);
    for (int i = 1; i < num_part+1; i++) {
        printf("*** %s\n", gera_nome_particao(i));
        imprime_arquivo(gera_nome_particao(i));
    }
}