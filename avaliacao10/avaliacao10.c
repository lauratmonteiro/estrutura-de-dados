#include <stdio.h>

int main(){
    int m;
    scanf("%d", &m);
    int tabela[m];
    int cont = 0;

    for(int i = 0; i<m; i++){
        tabela[i] = -1;
    }

    int chave;
    scanf("%d", &chave);

    while(chave >= 0){
        int h = chave % m;

        for(int i = 0; i<m; i++){

            int hxk = (h+i) % m;

            if(tabela[hxk] == -1){
                tabela[hxk] = chave;
                break;
            }

            cont += 1;
        }

        scanf("%d", &chave);
    }

    printf("%d", cont);

    //resposta da ultima questao:
    for(int i = 0; i<m; i++){
        printf("%d -> %d\n", i, tabela[i]);
    }

    return 0;
}