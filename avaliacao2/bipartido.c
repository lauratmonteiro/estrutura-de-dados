#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct vizinho {
    int id_vizinho;
    struct vizinho *prox;
}TVizinho;

/*
 * Funções e tipos auxiliares, para uso caso necessário
 */

typedef struct lista {
    int info;
    struct lista* prox;
} TLista;

typedef struct pilha{
    TLista *topo;
} TPilha;

TPilha *inicializa() {
    TPilha *pilha = (TPilha *)malloc(sizeof(TPilha));
    pilha->topo = NULL;
    return pilha;
}

void libera(TPilha *p) {
    TLista *q = p->topo;
    TLista *r;
    while(q != NULL){
        r = q;
        q=q->prox;
        free(r);
    }
    free(p);
}

int pilha_vazia(TPilha *pilha) {
    if (pilha->topo == NULL)
        return 1; //pilha vazia
    else
        return 0; //pilha tem pelo menos 1 elemento
}

/* *
 * Insere elem no topo da pilha
 * */
void push(TPilha *pilha, int elem) {
    TLista *novo = (TLista*) malloc(sizeof(TLista));
    novo->info = elem;
    novo->prox = pilha->topo;
    pilha->topo = novo;
}

/* *
 * Exclui o elemento do topo da pilha
 * retorna o info do elemento excluído
 */
int pop(TPilha *pilha) {
    if (pilha_vazia(pilha)) {
        exit(1);
    }
    else {
        TLista *aux = pilha->topo;
        int info = aux->info;
        pilha->topo = aux->prox;
        free(aux);
        return info;
    }
}

/* *
 * Consulta o elemento do topo da pilha
 * retorna info do elemento do topo ou -1 caso a pilha esteja vazia
 */
int peek(TPilha *pilha) {
    if (pilha_vazia(pilha))
        return -1;
    else {
        //faz consulta
        return pilha->topo->info;
    }
}

void imprime_pilha(TPilha *pilha) {
    int x;
    TPilha *aux = inicializa();
    while (!pilha_vazia(pilha)) {
        x = pop(pilha);
        printf("%d\n", x);
        push(aux, x);
    }
    while (!pilha_vazia(aux)) {
        push(pilha, pop(aux));
    }
    libera(aux);
    printf("\n");
}

/*
 * Funções de grafo
 */

typedef struct grafo{
    int id_vertice;
    int cor;
    TVizinho *prim_vizinho;
    struct grafo *prox;
}TGrafo;

TGrafo *insere_vertice(TGrafo *g, int id) {
    TGrafo *vertice = (TGrafo *) malloc(sizeof(TGrafo));
    vertice->id_vertice = id;
    vertice->cor = -1;
    vertice->prox = g;
    vertice->prim_vizinho = NULL;
    return vertice;
}

void libera_vizinho(TVizinho *vizinho) {
    if (vizinho != NULL) {
        libera_vizinho(vizinho->prox);
        free(vizinho);
    }
}

void libera_vertice(TGrafo *vertice) {
    if (vertice != NULL) {
        libera_vizinho(vertice->prim_vizinho);
        libera_vertice(vertice->prox);
        free(vertice);
    }
}

TGrafo *busca_vertice(TGrafo *vertice, int id) {
    while ((vertice != NULL) && (vertice->id_vertice != id)) {
        vertice = vertice->prox;
    }
    return vertice;
}

TVizinho *busca_vizinho(TVizinho *vizinho, int id) {
    while ((vizinho != NULL) && (vizinho->id_vizinho != id)) {
        vizinho = vizinho->prox;
    }
    return vizinho;
}

void insere_aresta(TGrafo *g, int origem, int destino) {
    TGrafo *vertice = busca_vertice(g, origem);
    TVizinho *vizinho = (TVizinho *) malloc(sizeof(TVizinho));
    vizinho->id_vizinho = destino;
    vizinho->prox = vertice->prim_vizinho;
    vertice->prim_vizinho = vizinho;
}

void imprime(TGrafo *vertice) {
    while (vertice != NULL) {
        printf("Vertice: %d\n", vertice->id_vertice);
        printf("Vizinhos: ");
        TVizinho *vizinho = vertice->prim_vizinho;
        while (vizinho != NULL) {
            printf("%d ", vizinho->id_vizinho);
            vizinho = vizinho->prox;
        }
        printf("\n\n");
        vertice = vertice->prox;
    }
}

int corigual(TGrafo *g, int cor) {
    if (g->cor == cor) return 1; 
    return 0; 
}

int coroposta(TGrafo *g, int cor) {
    if (g->cor != cor && g->cor != -1) return 1; 
    return 0; 
}

void pintacoroposta(TGrafo *g, int cor) {
    g->cor = 1-cor;
}

int bipartidoconexo(TGrafo *raiz, TGrafo *g, int cor) {
    TVizinho *v = g->prim_vizinho;
    while(v != NULL) {
        TGrafo *aux = busca_vertice(raiz, v->id_vizinho);

        if (corigual(aux, cor)) return 0;
        else if (!coroposta(aux, cor)) {
            pintacoroposta(aux, cor);
            if(!bipartidoconexo(raiz, aux, aux->cor)) return 0;
        }
        v = v->prox;
    }

    return 1;
}

int ehbipartido(TGrafo *g) {
    g->cor = 0; 
    if (!bipartidoconexo(g, g, 0)) {
        return 0;
    }

    TGrafo *aux = g;
    while (aux != NULL ) { 
        if (aux->cor == -1)
            if (!bipartidoconexo(g, aux, 0)) return 0;
        aux = aux->prox;
    }

    return 1;
}

int main() {
    /* A função main lê os dados de entrada, cria o grafo e chama a função solicitada no problema
    * depois imprime o resultado
    * ELA NÃO DEVE SER MODIFICADA
    * */
    int num_vertices, num_arestas;
    int id;
    int origem, destino;
    char l[100];
    char delimitador[] = "-";
    char *ptr;
    int i;
    TGrafo *g = NULL;

    //le numero de vertices
    scanf("%d", &num_vertices);
    //le e cria os vertices
    for (i = 0; i < num_vertices; i++) {
        scanf("%s", l);
        id = atoi(l);
        g = insere_vertice(g, id);
    }

    //Le numero de arestas e depois le os dados de cada aresta
    //Cria as arestas no grafo
    scanf("%d", &num_arestas);
    for (i = 0; i < num_arestas; i++) {
        scanf("%s", l);
        //quebra a string de entrada
        ptr = strtok(l, delimitador);
        origem = atoi(ptr);
        ptr = strtok(NULL, delimitador);
        destino = atoi(ptr);
        insere_aresta(g, origem, destino);
        insere_aresta(g, destino, origem);
    }

    //imprime(g);

    printf("%d", ehbipartido(g));
    libera_vertice(g);
}