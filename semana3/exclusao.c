#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNoA {
    int chave;
    struct sNoA *esq;
    struct sNoA *dir;
} TNoA;

TNoA *exclui(TNoA *raiz, int chave) {
    //TODO: Implementar essa função
    //Ela recebe a raiz da árvore e a chave a ser excluída
    //Ela retorna ponteiro para a nova raiz
    
    TNoA *ant = NULL;
    TNoA *aux = raiz;
    while(aux != NULL){
        if(aux->chave > chave){
            ant = aux;
            aux = aux->esq;
        }else if(aux->chave < chave){
            ant = aux;
            aux = aux->dir;
        }
        if(aux->chave == chave) break;
    }
    if(aux==NULL) return raiz;
    
    if(aux->dir == NULL && aux->esq == NULL){
        //caso 1: eh uma folha
        
        if(ant == NULL){
            raiz = NULL;
        }else{
            if(ant->esq == aux){
                ant->esq = NULL;
            }else if(ant->dir == aux){
                ant->dir = NULL;
            }
        }
        free(aux);
        return raiz;
    }

    if(aux->dir != NULL){
        if(aux->esq == NULL){
            //caso 2: arvore so na direita
            if(ant == NULL){
                //significa q estou na raiz
                raiz = aux->dir;
            }else{
                if(ant->esq == aux){
                    ant->esq = aux->dir;
                }else if(ant->dir == aux){
                    ant->dir = aux->dir;
                }

            }
            free(aux);
            return raiz;
        }else{
            //caso 3: no possui duas subarvores
            TNoA*aux2 = aux->esq;
            while(aux2->dir != NULL){
                aux2 = aux2->dir;
            }
            aux->chave = aux2->chave;
            aux->esq = exclui(aux->esq, aux->chave);
            return raiz;
        }

    }
    if(aux->esq != NULL){
        if(aux->dir == NULL){
            //caso 2: arvore so na esquerda
            if(ant == NULL){
                raiz = aux->esq;
            }else{
                if(ant->esq == aux){
                    ant->esq = aux->esq;
                }else if(ant->dir == aux){
                    ant->dir = aux->esq;
                }
            }
            
            free(aux);
            return raiz;
        }

    }
    return raiz;
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

    //le valor a ser excluido
    scanf("%d", &valor);
    //Chama função
    raiz = exclui(raiz, valor);
    imprime(raiz, 0);
};