#include <stdlib.h>
#include <stdio.h>

typedef struct{
  int id;
  int rrn;
}Registro;

int particiona(Registro *V, int inicio, int fim){

    int esquerda = inicio;
    int direita = fim;
    int pivo = V[inicio].id;

    while(esquerda<=direita){

        while(V[esquerda].id <= pivo){
        esquerda++;
        }

        while(V[direita].id > pivo){
            direita--;
        }

        if(esquerda < direita){

            Registro aux = V[esquerda];
            V[esquerda] = V[direita];
            V[direita] = aux;
        }
    }

    Registro aux =V[inicio];
    V[inicio] = V[direita];
    V[direita] = aux;

    return direita;
}

void quicksort(Registro *V, int inicio, int fim){
    int pivo;

    if(inicio < fim){
        pivo = particiona(V, inicio, fim);
        quicksort(V,inicio, pivo-1);
        quicksort(V, pivo+1, fim);
    }

} // função particiona



int main(){

char buffer[5];

    int tamanho_vetor;

    FILE *arq;

    arq = fopen("entrada.txt", "r");

    fseek(arq,0,SEEK_END);
    tamanho_vetor = ftell(arq);
    tamanho_vetor = tamanho_vetor/56;
    fseek(arq,0,SEEK_SET);

    Registro vetor[tamanho_vetor];

    for(int i =0; i<tamanho_vetor; i++){

          fread(buffer, 4,1 ,arq);
          vetor[i].id = atoi(buffer);
          vetor[i].rrn = i;
          fseek(arq, 52, SEEK_CUR);
    }

    quicksort(vetor,0,tamanho_vetor-1);

    for (size_t j = 0; j <tamanho_vetor ; j++) {
        printf("ID: %i\t", vetor[j].id );
        printf("RRN: %i\n", vetor[j].rrn );
    }

    

  return 0;
}
