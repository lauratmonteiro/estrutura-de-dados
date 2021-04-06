#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNoA {
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
} TNoA;

int altura(TNoA *raiz){
    if(raiz == NULL) return 0;
    if(raiz->dir == NULL && raiz->esq == NULL) return 1; //se for folha

    int alt_dir = altura(raiz->dir);
    int alt_esq = altura(raiz->esq);

    if(alt_dir > alt_esq) return 1+alt_dir;
    
    return 1+alt_esq;
}

int ehAVL(TNoA *raiz) {
    //TODO: Implementar essa função
    //Ela recebe a raiz da árvore
    //Ela retorna um inteiro (1 se a árvore for AVL, 0 caso contrário)
    if(raiz == NULL) return 1;
    if(ehAVL(raiz->dir) && ehAVL(raiz->esq)) {
        int fb = altura(raiz->dir) - altura(raiz->esq);
        if(fb >= -1 && fb <= 1) return 1;
    }
    
    return 0;
}

void imprime(TNoA *nodo, int tab) {
    for (int i = 0; i < tab; i++) {
        printf("-");
    }
    if (nodo != NULL) {
        printf("%d\n", nodo->chave);
        imprime(nodo->esq, tab + 2);
        printf("\n");
        imprime(nodo->dir, tab + 2);
    } else printf("vazio");
}

TNoA *insere(TNoA *no, int chave) {
    if (no == NULL) {
        no = (TNoA *) malloc(sizeof(TNoA));
        no->chave = chave;
        no->esq = NULL;
        no->dir = NULL;
    } else if (chave < (no->chave))
        no->esq = insere(no->esq, chave);
    else if (chave > (no->chave)) {
        no->dir = insere(no->dir, chave);
    } else {
        printf("Inserção inválida! ");
        exit(1);
    }
    return no;
}

int main(void) {

    /* A função main lê os dados de entrada, cria a árvore e chama a função solicitada no problema
     * depois imprime o resultado solicitado
     * ELA NÃO DEVE SER MODIFICADA
     * */
    TNoA *raiz;
    raiz = NULL;

    char l[100];
    char delimitador[] = "-";
    char *ptr;
    int valor;

    /* lê valores para criar a arvore
     * valores devem ser informados separados por traço
     * exemplo: 1-3-5-2-7-9-21-6 */
    scanf("%s", l);
    //quebra a string de entrada
    ptr = strtok(l, delimitador);
    while(ptr != NULL) {
        valor = atoi(ptr);
        raiz = insere(raiz, valor);
        ptr = strtok(NULL, delimitador);
    }

    //Chama função
    printf("%d", ehAVL(raiz));
};