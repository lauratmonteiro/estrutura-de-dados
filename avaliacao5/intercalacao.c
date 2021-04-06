#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "nomes.c"
#include "cliente.c"

#include <limits.h>
#include <dirent.h>
#include <string.h>

typedef struct vetor {
    TCliente *cli;
    FILE *f;
} TVet;

typedef struct vetorNomes {
    char *nome;
} TVetNomes;

TNomes *gera_nomes_particoes(int num){
    TVetNomes nomes[num];
    TNomes* lista_nomes_particoes = NULL;
    for (int i = num-1; i >= 0; i--) {
        nomes[i].nome = (char *) malloc(sizeof(char[10]));
        char num_particao[10];
        strcpy(nomes[i].nome, "p");
        sprintf(num_particao, "%d", i+1);
        strcat(nomes[i].nome, num_particao);
        strcat(nomes[i].nome, ".txt");
        lista_nomes_particoes = insere_inicio(lista_nomes_particoes, nomes[i].nome);
    }
    return lista_nomes_particoes;
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

void imprime_arquivo(char *name) {
    FILE *arq; //declara ponteiro para arquivo
    // abre arquivo para leitura
    arq = fopen(name, "r");
    if (arq != NULL) {// checa se não deu erro na abertura do arquivo
        char linha[150];
        fgets (linha, 150, arq);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s", linha);
            fgets (linha, 150, arq);
        }
        fclose(arq); //fecha arquivo
    } else printf("Erro ao abrir arquivo\n");
}

void preenche_clientes(TVet *vet, int tam){
    for(int i = 0; i<tam; i++){
        if(vet[i].cli == NULL){
            if(vet[i].f != NULL){
                vet[i].cli = le_cliente(vet[i].f);
            }
        }
    }
}

int retorna_pos_menor(TVet *vet, int tam){
    int menor = -1;
    for(int i = 0; i<tam; i++){
        if(vet[i].cli != NULL){
            menor = i;
            break;
        }
    }

    for(int i = menor+1; i<tam; i++){
        if(vet[i].cli != NULL && vet[i].cli->cod_cliente < vet[menor].cli->cod_cliente) menor = i;
    }

    return menor;

}

void fecha_arquivos(TVet *vet, int tam){
    for(int i = 0; i<tam; i++){
        if(vet[i].f != NULL){
            fclose(vet[i].f);
        }
    }
}

void printa_cliente(TVet *vet, int tam){
    for(int i = 0; i<tam; i++){
        if(vet[i].cli != NULL){
            printf("%d ", vet[i].cli->cod_cliente);
        }
    }
    printf("\n");
}

int num_part = 0;

int intercalacao_otima(char *nome_arquivo_saida, int num_p, TNomes *nome_particoes, int f) {
    if(num_p == 1){
        rename(nome_particoes->nome, nome_arquivo_saida);
        return 0;
    }

    TNomes *aux = nome_particoes;
    if(num_part == 0) {
        num_part = num_p+1;
    }
    
    int num_p_aux = 0;
    TNomes *lista_nomes = NULL;

    while(1){
        int num_nomes = conta_nomes(aux);

        if(num_nomes < f-1){
            while(aux){
                lista_nomes = insere_fim(lista_nomes, aux->nome);
                aux = aux->prox;
            }

            break;
        }

        TVet *vet = (TVet*)malloc(sizeof(TVet)*(f-1));

        for(int i = 0; i<f-1; i++){
            if(aux !=  NULL){
                FILE *arq = fopen(aux->nome, "r");
                vet[i].f = arq;
                aux = aux->prox;
            }else{
                vet[i].f = NULL;
            }

            vet[i].cli = NULL;
        }
        char *nome_part = gera_nome_particao(num_part++);
        FILE *part_aux = fopen(nome_part, "w");

        while(1){
            preenche_clientes(vet, f-1);
            int pos_menor = retorna_pos_menor(vet, f-1);
            if(pos_menor == -1){
                break;
            }
            salva_cliente(vet[pos_menor].cli, part_aux);
            free(vet[pos_menor].cli);
            vet[pos_menor].cli = NULL;
        }

        lista_nomes = insere_fim(lista_nomes, nome_part);
        fecha_arquivos(vet, f-1);
        fclose(part_aux);
        num_p_aux += 1;
        
        free(vet);
    }
    int qtd_lista = conta_nomes(lista_nomes);

    int p_rec = intercalacao_otima(nome_arquivo_saida, qtd_lista, lista_nomes, f);
    libera_nomes(lista_nomes);

    return num_p_aux + p_rec;
    
}

int main() {
    int num_particoes;
    int f;
    int num_particoes_intermediarias;
    TNomes *nomes_particoes = NULL;
    //le numero de particoes a serem intercaladas
    scanf("%d", &num_particoes);
    nomes_particoes = gera_nomes_particoes(num_particoes);
    //le f (número total de arquivos a serem manipulados ao mesmo tempo)
    //lembrar que o método usa f-1 arquivos para leitura, e 1 arquivo para saída
    scanf("%d", &f);
    num_particoes_intermediarias = intercalacao_otima("saida.txt", num_particoes, nomes_particoes, f);
    printf("%d\n", num_particoes_intermediarias);
    printf("*** saida.txt\n");
    imprime_arquivo("saida.txt");
}