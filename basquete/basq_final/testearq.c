#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "time.h"

#define num_k 1
#define num_testes 10


int main(){
	int numero_iteracao = 100;
	float percentf=0;
	float percenttheta=0;


	//PASSO O TXT COM OS DADOS PARA UMA MATRIZ
	while(numero_iteracao>0){

				 // abrindo arquivo
	    FILE *arqteste = fopen("tabelaresultados.txt","r");
	    float M[555][4];
	    int f = 0;
	    int g = 0;
	  
	    while(feof(arqteste)==0){

	    	fscanf(arqteste, "%f", &M[f][g]);

	    	g ++;
	    	if(g == 4){
	    		f++;
	    		g=0;
	    	}else{
	    		fseek(arqteste,1,SEEK_CUR);
	    	}


	    }

	    	fclose(arqteste);

	    // for(int i=0;i<555;i++){
	    // 	for(int j=0;j<4;j++){
	    // 		printf("%f ",M[i][j] );
	    // 	}

	    // 	printf("\n");
	    // }


	  

	    //ESCOLHO ALEATORIAMENTE OS INDICES Q SERAO USADOS PARA TESTE
	    srand((unsigned int)time(NULL));
	    int range = (555/num_testes);
	      int valoraleat = rand() % range;
	      int ind_teste[num_testes];
	     // printf("%d\n",valoraleat );

	    for(int i=0; i<num_testes ;i++){

	    	ind_teste[i] = valoraleat ;

	    	//printf("%d ",ind_teste[i] );
	  
	    	

	    }

//printf("\n");


	    //PREENCHO A MATRIZ TESTE COM OS DADOS DA PRIMEIRA MATRIZ GERADA
	    float Mteste[num_testes][4];

	    for(int i=0; i<num_testes; i++){
	    	for(int j=0; j<4; j++){

	    		Mteste[i][j]=M[ind_teste[i]][j];
	    		//printf("%f ",Mteste[i][j] );


	    	}
	    	//printf("\n");
	    }


	    //INSTANCIO E ZERO A MATRIZ Q VAI GUARDAR AS DISTANCIAS DOS TESTE COM OS TREINOS	    
	    float k_dist[num_testes][555];

	    for(int i=0;i<num_testes;i++){
	    	for(int j=0;j<555;j++){
	    		k_dist[i][j]=10;
	    	}
	    	
	    }



	  	//PRA CADA TESTE EU CALCULO A DISTANCIA DELE COM OS OUTROS 555 VALROES D TREINO(COM EXCESSAO AOS DADOS Q EU PEGUEI PRA TESTE)  
	    int alertatreino;
	    float a;
	    float b;


	     for(int i=0; i<num_testes; i++){//50
	    	for(int j=0; j<555 ; j++){//555
	    		alertatreino=0;

	    		for(int k=0;k<num_testes;k++){
	    			if(j==ind_teste[k]){
	    				
	    				alertatreino = 1;
	    			}
	    		}

	    		if(alertatreino==0){
	    			//k_dist[i][j]=0;
	    			//printf("%f\n", k_dist[i][j]);
	    			//k_dist[i][j]=sqrt(pow(fabs(Mteste[i][2] - M[j][2]),2) + pow(fabs(Mteste[i][3] - M[j][3]),2));
	    			k_dist[i][j]=sqrtf(pow(fabs(M[ind_teste[i]][2] - M[j][2]),2) + pow(fabs(M[ind_teste[i]][3] - M[j][3]),2));
	    			a=pow(fabs(M[ind_teste[i]][2] - M[j][2]),2) ;
	    			b=pow(fabs(Mteste[ind_teste[i]][3] - M[j][3]),2);
	    			//printf("A %f  B %f\n",a,b );
	    			//printf("%f   \n",pow(fabs(M[ind_teste[i]][2] - M[j][2]),2) + pow(fabs(Mteste[ind_teste[i]][3] - M[j][3]),2));
	    		//	printf("%f   %f\n",fabs(M[ind_teste[i]][3] - M[j][3]), pow(fabs(M[ind_teste[i]][3] - M[j][2]),3));
	    			//printf("%f\n", k_dist[i][j]);

	    		}

	    		
	    		//printf("%f \n",k_dist[i][j] );

	    	}

	    	//printf("\n");
	    	
	    }
	  // printf("\n");


	    //VASCULHO OS K VALORES COM MENOR DISTANCIA GUARDO O INDICE Q ELE C REFERE DA MATRIZ PRINCIPAL 
	    typedef struct{
	    	int indice;
	    	float valor;
		} struct_menores;

	    struct_menores menoresK[num_testes][num_k];
	    int ind_menor=0;

	    for(int i=0; i<num_testes; i++){




	    	for(int k=0; k<num_k; k++){

	    		ind_menor=0;

	    		for(int j=1; j<555; j++){

		    		if(k_dist[i][j]<k_dist[i][ind_menor]){
		    			ind_menor=j;
		    		}
		    		

	    		}

	    		menoresK[i][k].valor= k_dist[i][ind_menor];
	    		menoresK[i][k].indice=ind_menor;
	    		//printf("ind %d k %d %f \n",ind_menor,k,k_dist[i][ind_menor] );
		    	k_dist[i][ind_menor] = 10; //aqui eu aumento o valor do menor pra na proxima iteracao ele pegar o segundo menor valor e dps o terceiro menor valor e assim por diante

	    	}
	    	



	    }



	    //PRA CADA TESTE EU PEGO OS K MENORES VALROES DA MATRIZ INICIAL PELO INDICE Q SALVEI NO PASSO ANTERIOR E CALCULO A MEDIA
	    //PONHO ESSE VALOR COMO SENDO O VALOR PRO TESTE
	    float medianovovalor[num_testes][2];
	    float mediaf=0;
	    float mediatheta=0;
	    int ind_media;

	    for(int i=0; i<num_testes; i++){

	    	mediaf=0;
	    	mediatheta=0;

	    	for(int j=0; j<num_k; j++){
	    		ind_media=menoresK[i][j].indice;
	    		mediaf = mediaf + M[ind_media][0];
	    		mediatheta = mediatheta +  M[ind_media][1];

	    	}

	    	mediaf = mediaf/num_k;
	    	mediatheta = mediatheta/num_k;

	    	medianovovalor[i][0]=mediaf;
	    	medianovovalor[i][1]=mediatheta;

	    }




	   //CALCULO A DIFERENCA DO VALOR CALCULADO PELO VALOR REAL DO TREINO Q TA SALVO NA MATRIZ PRINCIPAL
	   float precisao_f=0;
	   float precisao_theta=0;
	   float totalf = 0;
	   float total_theta=0;

	    for(int i = 0; i<num_testes; i++){
	    	//printf("mediaf %f  valorf %f\n", medianovovalor[i][0],M[ind_teste[i]][0] );
	    	precisao_f = fabs(medianovovalor[i][0] - M[ind_teste[i]][0]);
	    	//printf("mediat %f  valort %f\n", medianovovalor[i][1],M[ind_teste[i]][1] );
	    	precisao_theta = fabs(medianovovalor[i][1] - M[ind_teste[i]][1]);
	    	totalf= totalf + precisao_f;
	    	total_theta= total_theta + precisao_theta;

	    	//printf("PRECISAO %d\nF: %f\nTHETA:%f\n\n",i,precisao_f,precisao_theta);


	    }

	   // printf("PRECISAO TOTAL\nF: %f\nTHETA:%f\n\n",(totalf/num_testes)*100/10,(total_theta/num_testes)*100/90);
	   // printf("rand %d\n",valoraleat );


	    percentf = percentf + (totalf/num_testes)*10;
	    percenttheta = percenttheta + (total_theta/num_testes)*10/9;
	    numero_iteracao --;
	    
  

	}


	percentf = percentf/100;
	percenttheta = percenttheta/100;

	printf("PERCENTS F-> erro:%f  acerto:%f\nPERCENTS TEHTA-> erro:%f  acerto:%f\n\n",percentf,(100-percentf),percenttheta,(100 - percenttheta) );





	return 0;
}

 
