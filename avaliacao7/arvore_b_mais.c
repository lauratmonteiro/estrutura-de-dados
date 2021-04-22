#include <stdio.h>
#include "metadados.c"
#include "no_interno.c"

void imprime_arquivos() {
    int i;
    //Imprime arquivo de metadados
    FILE *arq_m = fopen("metadados.dat", "rb");
    TMetadados *m = le_metadados(arq_m);
    printf("*** Arquivo de Metadados ***\n");
    imprime_metadados(m);
    fclose(arq_m);

    //Imprime arquivo de índice (nós internos da árvore)
    //A leitura não segue ordem específica -- os nós são lidos na ordem em que foram gravados no arquivo
    FILE *arq_i = fopen("indice.dat", "rb");
    printf("\n\n*** Arquivo de Indice ***\n");
    i = 0;
    TNoInterno *ni = le_no_interno(m->d, arq_i);
    while (ni != NULL) {
        printf("(((Endereco %d)))\n", i * tamanho_no_interno(m->d));
        imprime_no_interno(m->d, ni);
        ni = le_no_interno(m->d, arq_i);
        i += 1;
    }
    fclose(arq_i);

    //Imprime arquivo de dados (nós folha da árvore)
    //A leitura não segue ordem específica -- os nós são lidos na ordem em que foram gravados no arquivo
    FILE *arq_d = fopen("clientes.dat", "rb");
    printf("\n*** Arquivo de Dados ***\n");
    i = 0;
    TNoFolha *nf = le_no_folha(m->d, arq_d);
    while (nf != NULL) {
        printf("(((Endereco %d)))\n", i * tamanho_no_folha(m->d));
        imprime_no_folha(m->d, nf);
        nf = le_no_folha(m->d, arq_d);
        i += 1;
    }
    fclose(arq_d);
}

/*
 * Executa busca em Arquivos utilizando Arvore B+
 * Assumir que ponteiros para NULL têm valor -1
 *
 * chave: chave do cliente que esta sendo buscado
 * nome_arquivo_metadados: nome do arquivo binário que contem os metadados
 * nome_arquivo_indice: nome do arquivo binário de indice (que contem os nohs internos da arvore B+)
 * nome_arquivo_dados: nome do arquivo binário de dados (que contem as folhas da arvore B+)
 *
 * Retorna ponteiro para nó em que a chave está ou deveria estar
 */
int busca(int chave, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{   
    
    TMetadados *m = le_arq_metadados(nome_arquivo_metadados);
    int ptr = m->pont_raiz;
    int ptr_novo;

    FILE *meta = fopen(nome_arquivo_metadados, "rb");
    FILE *ind = fopen(nome_arquivo_indice, "rb");
    FILE *cli = fopen(nome_arquivo_dados, "rb");

    if(m->raiz_folha == 0){
        while(1){
            fseek(ind, ptr, SEEK_SET);

            TNoInterno *no_int = le_no_interno(m->d, ind);
            imprime_no_interno(m->d, no_int);

            int i;
            for(i = 0; i<no_int->m; i++){
                if(no_int->chaves[i] > chave){
                    ptr_novo = no_int->p[i];
                    break;
                }
            }
            if(i == no_int->m){
                ptr_novo = no_int->p[no_int->m];
            }

            ptr = ptr_novo;
            
            if(no_int->aponta_folha){
                break;
            }
        }
        fseek(cli, ptr, SEEK_SET);
        TNoFolha *no_folha = le_no_folha(m->d, cli);
        imprime_no_folha(m->d, no_folha);

    }else{
        fseek(cli, ptr, SEEK_SET);

        TNoFolha *no_folha = le_no_folha(m->d, cli);
        imprime_no_folha(m->d, no_folha);

    }

    fclose(meta);
    fclose(ind);
    fclose(cli);
	return ptr;
}

int main () {
    //Descomente essa linha de código caso deseje imprimir o conteúdo dos arquivos de entrada para analisar
    //o comportamento da sua implementação

    //imprime_arquivos();

    /* Le chave a ser buscada */
    int chave;

    scanf("%d", &chave);

    //Chama função de busca
    int pont = busca(chave, "metadados.dat", "indice.dat", "clientes.dat");

    //Imprime resultado da função
    printf("RESULTADO DA BUSCA: %d", pont);
}