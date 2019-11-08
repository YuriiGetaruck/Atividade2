#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct{
    int id;
    int rrn;
}Registro;  // struct usada para armazenar os ID e RRNs de cada cadastro

typedef struct{
    int id;
    char nome[29];
}SecundarioNome;

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



void IndiceSecundarioNome(int tamanho){ //funcao cria arquivo de indice secundario por nome fracamente ligado

  SecundarioNome vetor[tamanho];//utilizado para guardar o ID

  FILE *arq4;
  FILE *arq5;

  char buffer_nome[29];//salva o nome temporariamente
  char buffer_id[3];//salva o id temporariamente

  arq4 = fopen("indiceSecundario_NOME.txt","w"); //cria o arquivo para escrita
  if(arq4==NULL){
  printf("n abriu aquivo\n");
  }

  arq5 = fopen("DadosPilotoID.txt","r"); // abre o arquivo de indice para leitura
  if(arq5==NULL){
  printf("n abriu aquivo\n");
  }

  fseek(arq4,0,SEEK_SET);//define o ponteiro do arquivo para seu incio
  fseek(arq5,0,SEEK_SET);//define o ponteiro do arquivo para seu incio

  for(int i = 0; i<tamanho; i++){//coleta os dados e imprime no aquivo
    fread(buffer_id,4,1,arq5);
    vetor[i].id=atoi(buffer_id);
    fread(buffer_nome,29,1,arq5);
    for(int j=0;j<29;j++){//retira caracter idesejado
        buffer_nome[strcspn(buffer_nome, "#")] = '\0';
    }//for
    fprintf(arq4,"%s|%d\n",buffer_nome,vetor[i].id);
    fseek(arq5, 23,SEEK_CUR);
  }//for

  fflush(arq4);//garante atualização do arquivo
  fclose(arq4);//fecha o arquivo
  fclose(arq5);//fecha o arquivo
}

void IndiceSecundarioPais(int tamanho){//função cria indice secundario por pais fortemente ligado, recebe apenas o tamanho como parametro
    FILE *arq;
    FILE *arq2;
    char buffer[15];//salva temporariamente o pais

    arq= fopen("IndiceSecundarioPais.txt","w");
    arq2= fopen("entrada.txt","r");

    fseek(arq2,33,SEEK_SET);//define a posição do ponteiro no arquivo para o pais
    fseek(arq,0,SEEK_SET);//define a posição inicial no arquivo de escrita

    for(int i = 0; i<tamanho+1; i++){//coleta e imprimi os dados no arquivo
    fread(buffer,15,1,arq2);
    fseek(arq2, 41,SEEK_CUR);
    for(int j=0;j<15;j++){//retira caracter idesejado
        buffer[strcspn(buffer, "#")] = '\0';
    }
    fprintf(arq,"%s|%d\n",buffer,i);
  }

    fflush(arq);//atualiza arquivo
    fclose(arq);//fecha o arquivo
    fclose(arq2);//fecha o arquivo
}

void listatodos(int tamanho){//lista todos os dados

    FILE *arq;
    arq = fopen("DadosPilotoID.txt","r");

    char buffer_id[4];
    char buffer_nome[29];
    char buffer_pais[15];
    char buffer_titulos[1];
    char buffer_corridas[3];
    char buffer_poles[2];
    char buffer_vitorias[2];

    int id,titulos,corridas,poles,vitorias;

    fseek(arq,0,SEEK_SET);

    printf("ID\tNome\tPais\tTitulos\tCorridas\tPoles\tVitorias\n");

    for(int i=0; i<tamanho+1; i++){// imprime na tela do uuario os dados
        fread(buffer_id,4,1,arq);
        fread(buffer_nome,29,1,arq);
        fread(buffer_pais,15,1,arq);
        fread(buffer_titulos,1,1,arq);
        fread(buffer_corridas,3,1,arq);
        fread(buffer_poles,2,1,arq);
        fread(buffer_vitorias,2,1,arq);

        for(int j=0;j<29;j++){//limpa caracteres indesejados
            buffer_nome[strcspn(buffer_nome, "#")] = '\0';
            buffer_pais[strcspn(buffer_pais, "#")] = '\0';
        }

        for(int k=0;k<4;k++){//limpa caracteres indesejados
            buffer_titulos[strcspn(buffer_titulos, "#")] = '\0';
            buffer_corridas[strcspn(buffer_corridas, "#")] = '\0';
            buffer_poles[strcspn(buffer_poles, "#")] = '\0';
            buffer_vitorias[strcspn(buffer_vitorias, "#")] = '\0';
        }


        //converte os dados de char para int
        id=atoi(buffer_id);
        titulos=atoi(buffer_titulos);
        corridas=atoi(buffer_corridas);
        poles=atoi(buffer_poles);
        vitorias=atoi(buffer_vitorias);


        printf("%d %s|%s|%d|%d|%d|%d\n",id,buffer_nome,buffer_pais,titulos,corridas,poles,vitorias);
    }

    fclose(arq);//fecha arquivo
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

    IndiceSecundarioNome(tamanho_vetor);
    IndiceSecundarioPais(tamanho_vetor);

    //**************************************************************************

    bool controle = true;  // varivale de controle usada no while
    int op; //variavel que aramzena a opção escolhida pelo usuario
    do {  //do
        printf(" ========= Pilotos de Fórmula 1 ===========\n1 – Listar todos os dados em uma tabela\n2 – Pesquisar por Nome\n3 – Pesquisar por País\n4 – Sair do programa\nDigite sua opção: ");
        scanf("%i", &op); //scanf da opção selecionada

        switch (op)
        {
        case 1:
            listatodos(tamanho_vetor);
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
