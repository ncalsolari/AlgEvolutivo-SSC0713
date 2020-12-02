#ifndef MAIN_H
#define MAIN_H




static void genocidio(){
    float rand_theta = 90;
    float rand_forca = 10;

    int ind_melhor = vetor_melhor_todos[0];


    for(int j=0; j<populacao; j++){
        if(j!=ind_melhor){

            vetor_theta[j]= ((float)rand()/(float)(RAND_MAX)) * rand_theta;
            vetor_forca_original[j]= ((float)rand()/(float)(RAND_MAX)) * rand_forca;

        }
    }

}


static void pred_sintese(){

    int ind_pior=0;
    int ind_melhor = vetor_melhor_todos[0];
    float media_forca=0;
    float media_theta=0;

    //acha o pior
    for (int j=1; j<populacao; j++){

        if(j!=ind_melhor){

            if(vetor_distfinal[j]>vetor_distfinal[ind_pior]){
             ind_pior=j;
            }

        }
        

    }
    //faz uma media do resto da pop sem contar o melhor e o pior
    for (int j=0; j<populacao; j++){
        if(j!=ind_melhor && j!=ind_pior){
            printf("valor forca %f\n",vetor_forca_original[j]);

            media_forca = media_forca + vetor_forca_original[j];
            media_theta = media_theta + vetor_theta[j];
        }

    }

    media_forca = media_forca/(populacao-2); //media nao leva em consideracao melhor valor nem pior valor
    media_theta = media_theta/(populacao-2);
    printf("ind pior: %d\n media f %f\nmedia t %f\n\n",ind_pior, media_forca, media_theta );

    //substitui o pior pela sintese
    vetor_forca_original[ind_pior]=media_forca;
    vetor_theta[ind_pior] = media_theta;



}

static void nova_geracao(){


    printf("melhor[%.0f]: %f\n\n\n",vetor_melhor_todos[0], vetor_melhor_todos[1] );
    int ind_melhor;
    float theta_melhor;
    float forca_melhor;
    int chance_mutacao;
    int gene_mutado;
    srand(time(NULL));

    ind_melhor = vetor_melhor_todos[0];

    //PEGO DADOS DO MELHOR
    theta_melhor = vetor_theta[ind_melhor];
    forca_melhor = vetor_forca_original[ind_melhor];

    //CROSSOVER
    for(int j = 0; j<populacao; j++){



        if(j!= ind_melhor){
            vetor_cesta_acerto[j]=100; //zero a avaliacao 
           // vetor_cesta_acertotxt[j]=100; //zero a avaliacao 
            vetor_theta[j]= (vetor_theta[j] + theta_melhor) / 2;
            vetor_forca_original[j]= (vetor_forca_original[j] + forca_melhor)/2;
        }


    }
    
    //MUTACAO E REARRANJO DA POPULACAO
    for(int j=0; j<populacao; j++){

       // srand(time(NULL)); srand null aqui faz com q todos os individuos tenham o msmo valor d chance d mutacao e gene mutado
       

         chance_mutacao = rand()%100;
         gene_mutado = rand()%100;
        
        
  

         
         if(j != ind_melhor){

            if(gene_mutado<=50){//MUTAR O THETA

            if(chance_mutacao<=50){ //50% D CHANCE D MUTAR PRA MAIS OU PRA MENOS
                vetor_theta[j]=vetor_theta[j]*(1 + percent_mutacao);
            }else{
                vetor_theta[j]=vetor_theta[j]*(1- percent_mutacao);
            }

         }else{//MUDAR A FORCA

            if(chance_mutacao<=50){ //50% D CHANCE D MUTAR PRA MAIS OU PRA MENOS
                vetor_forca_original[j]=vetor_forca_original[j]*(1 + percent_mutacao);
            }else{
                vetor_forca_original[j]=vetor_forca_original[j]*(1 - percent_mutacao);
            }

         }

         }
         
    }



}


static void resetapop(){

    printf("\nGeracao %d\nTaxa de mutacao %f\nDistancia do melhor %f\n", count_geracao,percent_mutacao,vetor_melhor_todos[1]);
            count_geracao ++;
           
            tempoteste = 0;
            pred_sintese();
        

            if(predacao == 15){
                
                printf("pred number: %d\n",predacao );
                printf("genocidio\n");
                genocidio();
                count_genocidios += 1;
                predacao = 0;
            }else{
                printf("pred number: %d\n",predacao );
                printf("NOVA GERACAO\n");
                nova_geracao();
                predacao = predacao + 1;
            }

                printf("teste numero: %d\n", treinos );
          

            for(int j=0; j<populacao;j++){

                matriz_forca_atual[j][0]= cos(vetor_theta[j]*pi/180)*vetor_forca_original[j];
                matriz_forca_atual[j][1]= sin(vetor_theta[j]*pi/180)*vetor_forca_original[j];

                matriz_tempo[j][0]=speed_time;
                matriz_tempo[j][1]=speed_time;

                matriz_pos_inicial[j][0]= posinicialX; //-0.799995;
                matriz_pos_inicial[j][1]= posinicialY; //-0.79415;

                matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];
                matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;

                subindo[j]=1;
                bolaviva[j]=1;

                matriz_quina_acerto[j][0]=0;
                vetor_distfinal[j]=1000;

            }




}



// funcao para processar eventos de teclado
static void key_event(GLFWwindow* window, int key, int scancode, int action, int mods){
   

    if(key==262 && action == GLFW_PRESS) percent_mutacao += 0.01; // tecla para direita
    if(key==263 && action == GLFW_PRESS) percent_mutacao -= 0.01; // tecla para direita

    if(key==265){// tecla para cima

        vetor_transldado_cesta[1] += 0.1; // tecla para cima
        vetor_melhor_todos[0]=-10;
        vetor_melhor_todos[1]=1000000;
        genocidio();
        predacao = 0;

        for(int j=0; j<populacao;j++){

            matriz_forca_atual[j][0]= cos(vetor_theta[j]*pi/180)*vetor_forca_original[j];
            matriz_forca_atual[j][1]= sin(vetor_theta[j]*pi/180)*vetor_forca_original[j];

            matriz_tempo[j][0]=speed_time;
            matriz_tempo[j][1]=speed_time;

            matriz_pos_inicial[j][0]= posinicialX; //-0.799995;
            matriz_pos_inicial[j][1]= posinicialY; //-0.79415;

            matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];
            matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;

            subindo[j]=1;
            bolaviva[j]=1;

            matriz_quina_acerto[j][0]=0;
            vetor_distfinal[j]=1000;

            
           

        }

    }

    if(key==264){  // tecla para baixo

        vetor_transldado_cesta[1] -= 0.1;
        vetor_melhor_todos[0]=-10;
        vetor_melhor_todos[1]=1000000;
        genocidio();
        predacao = 0;

        for(int j=0; j<populacao;j++){

            matriz_forca_atual[j][0]= cos(vetor_theta[j]*pi/180)*vetor_forca_original[j];
            matriz_forca_atual[j][1]= sin(vetor_theta[j]*pi/180)*vetor_forca_original[j];

            matriz_tempo[j][0]=speed_time;
            matriz_tempo[j][1]=speed_time;

            matriz_pos_inicial[j][0]= posinicialX; //-0.799995;
            matriz_pos_inicial[j][1]= posinicialY; //-0.79415;

            matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];
            matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;

            subindo[j]=1;
            bolaviva[j]=1;

            matriz_quina_acerto[j][0]=0;

            
           

        }

    }

    if(key==32 && action == GLFW_PRESS){
        printf("\nGeracao %d\nTaxa de mutacao %f\nDistancia do melhor %f\n", count_geracao,percent_mutacao,vetor_melhor_todos[1]);
         //printf("Melhor[%f]: %f\n",vetor_melhor_todos[0], vetor_melhor_todos[1] );
        // printf("angulo %f forca %f\n",vetor_theta[(int)vetor_melhor_todos[0]], vetor_forca_original[(int)vetor_melhor_todos[0]] );
        count_geracao ++;
        tempoteste = 0;
        
    
      /*
        if(predacao == 5){
            printf("pred number: %d\n",predacao );
            printf("SINTESE\n");
            pred_sintese();
            predacao = predacao + 1;
        }else{
            if(predacao == 15){
                
                printf("pred number: %d\n",predacao );
                printf("genocidio\n");
                genocidio();
                predacao = 0;
            }else{
                printf("pred number: %d\n",predacao );
                printf("NOVA GERACAO\n");
                nova_geracao();
                predacao = predacao + 1;
            }
           
        }
        */
        pred_sintese();
        

        if(predacao == 15){
            
            printf("pred number: %d\n",predacao );
            printf("genocidio\n");
            genocidio();
            predacao = 0;
        }else{
            printf("pred number: %d\n",predacao );
            printf("NOVA GERACAO\n");
            nova_geracao();
            predacao = predacao + 1;
        }
    

       

        for(int j=0; j<populacao;j++){

            matriz_forca_atual[j][0]= cos(vetor_theta[j]*pi/180)*vetor_forca_original[j];
            matriz_forca_atual[j][1]= sin(vetor_theta[j]*pi/180)*vetor_forca_original[j];

            matriz_tempo[j][0]=speed_time;
            matriz_tempo[j][1]=speed_time;

            matriz_pos_inicial[j][0]= posinicialX; //-0.799995;
            matriz_pos_inicial[j][1]= posinicialY; //-0.79415;

            matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];
            matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;

            subindo[j]=1;
            bolaviva[j]=1;

            matriz_quina_acerto[j][0]=0;
            vetor_distfinal[j]=1000;

            
           

        }

   
    }

}


static void leteste(){

}

static void knn(){


	//PASSO O TXT COM OS DADOS PARA UMA MATRIZ
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


    //INSTANCIO E ZERO A MATRIZ Q VAI GUARDAR AS DISTANCIAS DOS TESTE COM OS TREINOS	    
    float k_dist[555];

   
	for(int j=0;j<555;j++){
		k_dist[j]=10;
	}
    	
    

  	//PRA CADA TESTE EU CALCULO A DISTANCIA DELE COM OS OUTROS 555 VALROES D TREINO(COM EXCESSAO AOS DADOS Q EU PEGUEI PRA TESTE)  
    float a;
    float b;
	for(int j=0; j<555 ; j++){//555
		alertatreino=0;
		
		//k_dist[i][j]=0;
		//printf("%f\n", k_dist[i][j]);
		//k_dist[i][j]=sqrt(pow(fabs(Mteste[i][2] - M[j][2]),2) + pow(fabs(Mteste[i][3] - M[j][3]),2));
		k_dist[j]=sqrtf(pow(fabs(vetor_transldado_cesta[0] - M[j][2]),2) + pow(fabs(vetor_transldado_cesta[1] - M[j][3]),2));
		a=pow(fabs(M[ind_teste[i]][2] - M[j][2]),2) ;
		b=pow(fabs(Mteste[ind_teste[i]][3] - M[j][3]),2);
		//printf("A %f  B %f\n",a,b );
		//printf("%f   \n",pow(fabs(M[ind_teste[i]][2] - M[j][2]),2) + pow(fabs(Mteste[ind_teste[i]][3] - M[j][3]),2));
	    //	printf("%f   %f\n",fabs(M[ind_teste[i]][3] - M[j][3]), pow(fabs(M[ind_teste[i]][3] - M[j][2]),3));
		//printf("%f\n", k_dist[i][j]);

	
		
		//printf("%f \n",k_dist[i][j] );

	}

	

    //VASCULHO OS K VALORES COM MENOR DISTANCIA GUARDO O INDICE Q ELE C REFERE DA MATRIZ PRINCIPAL 
    typedef struct{
    	int indice;
    	float valor;
	} struct_menores;

    struct_menores menoresK[num_k];
    int ind_menor=0;



	for(int k=0; k<num_k; k++){

		ind_menor=0;

		for(int j=1; j<555; j++){

    		if(k_dist[i][j]<k_dist[i][ind_menor]){
    			ind_menor=j;
    		}
    		

		}

		menoresK[k].valor= k_dist[i][ind_menor];
		menoresK[k].indice=ind_menor;
		//printf("ind %d k %d %f \n",ind_menor,k,k_dist[i][ind_menor] );
    	k_dist[i][ind_menor] = 10; //aqui eu aumento o valor do menor pra na proxima iteracao ele pegar o segundo menor valor e dps o terceiro menor valor e assim por diante

	}
    	



    //PRA CADA TESTE EU PEGO OS K MENORES VALROES DA MATRIZ INICIAL PELO INDICE Q SALVEI NO PASSO ANTERIOR E CALCULO A MEDIA
    //PONHO ESSE VALOR COMO SENDO O VALOR PRO TESTE
    float medianovovalor[2];
    float mediaf=0;
    float mediatheta=0;
    int ind_media;

   
	for(int j=0; j<num_k; j++){
		ind_media=menoresK[j].indice;
		mediaf = mediaf + M[ind_media][0];
		mediatheta = mediatheta +  M[ind_media][1];

	}

	mediaf = mediaf/num_k;
	mediatheta = mediatheta/num_k;

	vetor_forca_original[0]=mediaf;
	vetor_forca_original[1]=mediaftheta

	medianovovalor[0]=mediaf;
	medianovovalor[1]=mediatheta;

    

}