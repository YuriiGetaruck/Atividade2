#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
    int id;
    int rrn;
}Registro;  // struct usada para armazenar os ID e RRNs de cada cadastro

int particiona(Registro *V, int inicio, int fim){  //funcao usada pela quicksort

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

void quicksort(Registro *V, int inicio, int fim){ //funcao quick sort
    int pivo;

    if(inicio < fim){
        pivo = particiona(V, inicio, fim);
        quicksort(V,inicio, pivo-1);
        quicksort(V, pivo+1, fim);
    }

}

void criaIndice(FILE *arq, int tamanho, Registro *vetor){  //funcao que cria o indice primario ordenado pelo ID

    arq = fopen("indicePrimario_ID", "w"); //Cria o arquivo de indice em modo de escrita

    for (int j = 0; j < tamanho ; ++j) {

        fprintf(arq, "%i|%i \n", vetor[j].id, vetor[j].rrn);  //insere no arquivo de indice o ID e RRN
    }

    fflush(arq);  //garante a passagem de todas as informaçoes pro arquivo
    fclose(arq);  //fecha o arquivo

}



void IndiceSecundarioNome(FILE *arq,FILE *arq2, int tamanho){ //funcao cria arquivo de indice secundario por nome fracamente ligado

  char buffer_nome[29];
  int buffer_id[3];

  int tamanho_campo_nome=29;//tamanho do campo "nome" no arquivo de dados
  int tamanho_campo_id=4;

  arq = fopen("indiceSecundario_NOME","w"); //cria o arquivo para escrita

  arq2 = fopen("DadosPilotoID","r"); // abre o arquivo de indice para leitura

  fseek(arq,0,SEEK_SET);
  fseek(arq2,0,SEEK_SET);

  for(int i = 0; i<tamanho*56; i++){
    fseek(arq2,0,(SEEK_SET*i*56+4));//coloca o marcador ja no nome do piloto

    for(int j=0; j<tamanho_campo_nome; j++){
      fscanf(arq2,"%s",buffer_nome[j]);//salva o nome no buffer_nome
    }
    for(int j=0; j<tamanho_campo_id; j++){
      fscanf(arq2,"%s",buffer_id[j]);//salva o id no buffer_id
    }

    fprintf(arq,"%s|",buffer_nome);
    for(int k=0;k>tamanho_campo_nome;k++){
    fprintf(arq,"%d",buffer_id[k]);
    }
    fprintf(arq,"\n");
  }

  fflush(arq);
  fclose(arq);
  fclose(arq2);

}


int main(){

    //Ordenas struct com os ID e RRN ***************************************
    char buffer[5];  //buffer para armazenar o numero do indice
    int tamanho_vetor;  // varaivel para armazenar quantos cadastros tem no arquivo

    FILE *arq; //variavel para abrir o arquivo

    arq = fopen("entrada.txt", "r");

    fseek(arq,0,SEEK_END); //posicionar o "ponteiro" do arquivo para o final dele
    tamanho_vetor = ftell(arq); // pegar o numero de bytes do arquivo
    tamanho_vetor = tamanho_vetor/56; //como cada registro tem 56 bytes, então o total/56 resulta na quantidade de registros
    fseek(arq,0,SEEK_SET); // voltar o "ponteiro" para o começo do arquivo

    Registro vetor[tamanho_vetor];  //inicia um vetor do tipo Registros para armazenar os IDS e RRNs

    for(int i =0; i<tamanho_vetor; i++){

        fread(buffer, 4,1 ,arq); //ler o ID do registro
        vetor[i].id = atoi(buffer); //converte a string para int e armazena na struct
        vetor[i].rrn = i; // o RRN de cada regsitro, ou seja a posição no arquivo original
        fseek(arq, 52, SEEK_CUR); // como leu 4 bytes, avança mais 52 para ir pro proximo registro
    }

    quicksort(vetor,0,tamanho_vetor-1); //ordena o vetor de Registro pelo ID de cada posição
    fclose(arq); //fechar o arquivo

    //**********************************************************************

    criaIndice(arq, tamanho_vetor, vetor); //função que cria o indice primario ordenado pelo ID dos registros

    //Tarefa 1 *****************************************************************

    FILE *arq2; // varaivel necessaria para criar o arquivo ordenado pelo registro
    FILE *arq3; // variavel necessaria para criar o arquivo de incice secundario por nome

    arq = fopen("entrada.txt", "r"); //abre o arquivo original em modo leitura
    arq2 = fopen("DadosPilotoID.txt", "w"); //abre o arquivo novo em modo escrita

    char registro[57]; //buffer para armazenar cada registro do arquivo original

    for (int k = 0; k < tamanho_vetor ; ++k) {  // for que vai até acabar os registros

        fseek(arq, 56 * vetor[k].rrn, SEEK_SET); //posiciona o ponteiro do arquivo original na posição do registro solicitado pelo RRN do msm
        fgets(registro, 57, arq);  //copia o registro(56 bytes)
        fprintf(arq2, "%s", registro); //copia o registro no arquivo novo, como a struct de registro já está ordenada pelo ID, o arquivo novo também estará
    }
    fflush(arq2); //garante a passagem de todas as informaçoes pro arquivo novo
    fclose(arq); // fecha o arquivo 1
    fclose(arq2); //fecha o arquivo 2

    //Tarefa 3 *****************************************************************

    //IndiceSecundarioNome(arq3,arq2,tamanho_vetor);

    //**************************************************************************

    bool controle = true;  // varivale de controle usada no while
    int op; //variavel que aramzena a opção escolhida pelo usuario
    do {  //do
        printf(" ========= Pilotos de Fórmula 1 ===========\n1 – Listar todos os dados em uma tabela\n2 – Pesquisar por Nome\n3 – Pesquisar por País\n4 – Sair do programa\nDigite sua opção: ");
        scanf("%i", &op); //scanf da opção selecionada

        switch (op)
        {
        case 1:

        case 2:

            break;

        case 3:

            break;

        case 4:
            controle = false;  //muda a variavel de controle para falsa, logo o while irá parar, terminando o programa.
            printf("Saindo...");
            break;
        default:
            printf ("\nOpcao invalida, tente novamente\n");             //controle de erro para opções inválidas
            break;
        }

    } while (controle);  //while

    return 0; //return da função main
}
