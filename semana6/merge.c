#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imprime_arquivo(char *nomeArq){
    FILE *arq; //declara ponteiro para arquivo
    //abre arquivo para leitura
    arq = fopen(nomeArq, "r");
    if (arq != NULL){// checa se não deu erro na abertura do arquivo
        char s[10];
        fscanf(arq, "%s", s);
        while (!feof(arq)) {//testa se chegou ao final do arquivo
            printf("%s\n", s);
            fscanf(arq, "%s", s);
        }
        fclose(arq); //fecha arquivo
    }
    else printf("Erro ao abrir arquivo\n");
}

void merge(char *nomeArq1, char *nomeArq2, char *nomeArqMerge) {
    FILE *arq1 = fopen(nomeArq1, "r");
    FILE *arq2 = fopen(nomeArq2, "r");
    FILE *merge = fopen(nomeArqMerge, "w");
    char linha1[3];
    char linha2[3];
    int ultLinha;
    
    if(arq1 != NULL && arq2 != NULL){
        int resLinha1 = fscanf(arq1, "%s", linha1);
        int resLinha2 = fscanf(arq2, "%s", linha2);

        while(1){

            if(resLinha1 == EOF) {
                while(resLinha2 != EOF) {
                    if (atoi(linha2) == ultLinha) {
                        resLinha2 = fscanf(arq2, "%s", linha2);
                        continue;
                    }

                    fprintf(merge, "%s\n", linha2);
                    ultLinha = atoi(linha2);
                    resLinha2 = fscanf(arq2, "%s", linha2);
                }

                break;
            }

            if(resLinha2 == EOF) {
                while(resLinha1 != EOF) {
                    if (atoi(linha1) == ultLinha) {
                        resLinha1 = fscanf(arq1, "%s", linha1);
                        continue;
                    }

                    fprintf(merge, "%s\n", linha1);
                    ultLinha = atoi(linha1);
                    resLinha1 = fscanf(arq1, "%s", linha1);
                }

                break;
            }
            
            if(atoi(linha1) == atoi(linha2)){
                fprintf(merge, "%s\n", linha1);
                ultLinha = atoi(linha1);
                resLinha1 = fscanf(arq1, "%s", linha1);
                resLinha2 = fscanf(arq2, "%s", linha2);
            }else if(atoi(linha1) < atoi(linha2)){
                fprintf(merge, "%s\n", linha1);
                ultLinha = atoi(linha1);
                resLinha1 = fscanf(arq1, "%s", linha1);
            }else{
                fprintf(merge, "%s\n", linha2);
                ultLinha = atoi(linha2);
                resLinha2 = fscanf(arq2, "%s", linha2);
            }
        }
    }

    fclose(arq1);
    fclose(arq2);
    fclose(merge);
}

int main(int argc, char **argv) {
    merge("numeros1.txt", "numeros2.txt", "merge.txt");
    imprime_arquivo("merge.txt");
}
