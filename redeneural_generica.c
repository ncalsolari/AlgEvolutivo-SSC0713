/*
EXEMPLO BASICO DE UMA REDE NEURAL GENERICA COM 3 CAMADAS (ENTRADA, INTERMEDIARIA E SAIDA)

ESSE CODIGO FOI UMA ADAPTACAO DE UMA REDE NEURAL APLICADA EM OUTRO PROGRAMA
SUA EFICIENCIA NAO FOI TESTADA EM NENHUMA BASE JA PRE EXISTENTE

COMPILAR COM -lm
EXEMPLO: gcc redeneural_generica.c -lm -o redegenerica
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void rede_neural( int x, int y, int l, int c, int e, float resposta[c], float pesocamada1[x][y], float pesocamada2[l][c], float entrada[e]){

    //pesocamada1 -> x eh o nmero de entradas e y o numero de neuronios na camada intermediaria
    float intermediario[y];
    //pesocamada2 -> l eh o nmero de neuronios intermediarios e c o nmero de saidas

    
          
    for(int a = 0; a<y; a++){
        for(int b=0; b<e; b++){

            intermediario[a]= entrada[b]*pesocamada1[b][a];
            intermediario[a] = 1/(1+expf(-intermediario[a])); // funcao sigmoid - normaliza entre valores 0 e 1, nao eh a solucao mais eficiente mas eh uma solucao bem aceita e ainda usada
            printf("intermediario: %f\n", intermediario[a] );
 
        }
          
    }

          

        printf("\n");


    for(int a = 0;a<c;a++){
        for(int b = 0; b<y; b++){
            resposta[a]= resposta[a] + intermediario[b]*pesocamada2[b][a];
        }

        resposta[a]= 1/(1+expf(-resposta[a])); // normalizacao por sigmoid
        printf("resposta %f\n",resposta[a]);
       
    }
    // o 'for' que calcula os intermediarios poderia ser simplificado para um 'for' duplo igual esse
    // caso as entradas viessem em um vetor, ambas sao solucoes viaveis dependentes da aplicacao



}


int x = 4; // entradas
int y = 3; // neuronios intermediarios
int l = 3; // l sempre sera igual a y, a distinçao das 2 variaveis eh apenas para cunho didatico
int c = 2; // saidas
int e = 4; // entradas, dnovo sempre sera igual a xa disticao eh apenas para facilitar o entendimento



int main(){

    float pesocamada1[x][y];
    float pesocamada2[l][c];
    float resposta[c];
    float entrada[e];

    

    //GERANDO OS PESOS ALEATORIAMENTE ENTRE -3 E 3
    srand((unsigned int)time(NULL));
    float input;
    for(int i=0; i<e; i++){
        input=rand()%10;
        entrada[i]= input;
        printf("entrada: %f", entrada[i]);
    }

        printf("\n");

    float weight;
    for(int i=0;i<x;i++){
        for(int j=0;j<y;j++){
            
            weight=rand()%600;
            weight = (weight-300)/100;
            pesocamada1[i][j]= weight;
        }
       
    }


     for(int i=0;i<l;i++){
        for(int j=0;j<c;j++){
            
            weight=rand()%600;
            weight = (weight-300)/100;
            pesocamada2[i][j]= weight;
            
        }
    }

    rede_neural(x,y,l,c,e,resposta,pesocamada1,pesocamada2,entrada);

    return 0;

}