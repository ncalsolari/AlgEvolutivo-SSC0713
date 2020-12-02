/* para linux, instalar os pacotes libglfw3-dev mesa-common-dev libglew-dev */
/* para compilar no linux: gcc golfinho.c -lglfw -lGL -lGLEW -lm -o golfinho.bin */



#include <GL/glew.h>  

#define GLFW_INCLUDE_NONE
#define GLFW_FALSE   0
#define speed_time 0.005
#define pop_golfinho 200
#define pop_peixe  1
#define vel_global 0.02


#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


 


// variaveis globais
typedef struct{
    float x, y;
} coordenadas;

typedef struct{
    float veloc_atual;
    float direcUP;
    float direcDOWN;
    float direcRIGHT;
    float direcLEFT;
    float vida;
    int rede; //booleana, se ele ta fazendo a rede ou nao   
    float posX;
    float posY;
    float distXpeixe[pop_peixe];
    float distYpeixe[pop_peixe];
    int contato;
    
    float score;
} golfinho;

typedef struct{
    float veloc_atual;
    float direcUP;
    float direcDOWN;
    float direcRIGHT;
    float direcLEFT;
    int vida;
    float posX;
    float posY;
} peixe;


    golfinho tobias[pop_golfinho];



    peixe pacu[pop_peixe];


    // float matriz_pos_inicial[populacao][2]; // cada linha um individuo, coluna 0 = x coluna 1 = y
    // float matriz_tempo[populacao][2];
    // float matriz_forca_atual[populacao][2];
    // float matriz_cesta_atual[populacao][2];
    // float matriz_bola_atual[populacao][2];
    // float matriz_translado[populacao][2];
    // float matriz_translado_parcial[populacao][2];
    // float matriz_quina_acerto[populacao][1];
    // float vetor_forca_original[populacao];
    // float vetor_theta[populacao];
    // float vetor_cesta_acerto[populacao];
    // float vetor_cesta_acertotxt[populacao];
    // float vetor_melhor_todos[2]; // coluna 0 o indice coluna 1 o valor
    // float distanciax;
    // float distanciay;
    // float subindo[populacao];
     float pi = 3.14;
     float percent_mutacao = 0.02;
     int count_geracao = 1;
     int melhor=0;
     int controlenovageracao =0;
     float t_peixe_x=0;
     float t_peixe_y=0;
    // int predacao = 0;

    float vetor_transicao[4];
    float wcamada1[7 + 2*(pop_peixe)][4][pop_golfinho];
    float wcamada2[4][6][pop_golfinho];

static void rede_neural( int pop, float resposta[4], int golfinho, float pesocamada1[7 + 2*(pop_peixe)][4][pop], float pesocamada2[4][6][pop], float vel_final,float direc_finalUP, float direc_finalDOWN, float direc_finalRIGHT, float direc_finalLEFT, float rede_final,float fome_final,float distXpeixe[pop_peixe],float distYpeixe[pop_peixe]){

  
    float intermediario[4];

    for(int j = 0;j<4 ;j++){
        
        intermediario[j]= 0;
 
    }
    
    for(int j=0 ; j< 4;j++){
        for(int k=0; k<pop_peixe;k++){
            intermediario[j]= intermediario[j] + distXpeixe[k]*pesocamada1[7 + k][j][golfinho] + distYpeixe[k]*pesocamada1[7 + pop_peixe+k][j][golfinho];
        }
        
    }

    for(int j = 0;j<4 ;j++){
        
        intermediario[j]= intermediario[j] + vel_final*pesocamada1[0][j][golfinho] + direc_finalUP*pesocamada1[1][j][golfinho] + direc_finalDOWN*pesocamada1[2][j][golfinho] + direc_finalRIGHT*pesocamada1[3][j][golfinho] + direc_finalLEFT*pesocamada1[4][j][golfinho] + fome_final*pesocamada1[5][j][golfinho] + rede_final*pesocamada1[6][j][golfinho] ;
        intermediario[j] = 1/(1+expf(-intermediario[j]));
       // printf("intermediario: %f\n", intermediario[j] );
 
    }

    
    //printf("\n");


    for(int i = 0;i<6;i++){

        resposta[i]= intermediario[0]*pesocamada2[0][i][golfinho] + intermediario[1]*pesocamada2[1][i][golfinho] + intermediario[2]*pesocamada2[2][i][golfinho] + intermediario[3]*pesocamada2[3][i][golfinho];
        resposta[i]= 1/(1+expf(-resposta[i]));
       // printf("resposta %f\n",resposta[i] );
    }


}

static void atualiza_matriz(float *pont_m,float t_x,float t_y){

    *(pont_m+3)=t_x;
    *(pont_m+7)=t_y;


}

static void nova_geracao(){


  
    int ind_melhor=0;
    float peso_melhor[7 + 2*(pop_peixe)][4];
    float peso2_melhor[4][6];
    int gene_mutado_i;
    int gene_mutado_j;
    int ad_sub_mutado;
    int matrizpeso_mutada;
    int total_genes1 = 7 + 2*(pop_peixe) * 4;
    int total_genes2 = 4*6;
    srand(time(NULL));

   

    //PEGO DADOS DO MELHOR
    for(int j=1; j<pop_golfinho; j++){
        if(tobias[j].score>tobias[ind_melhor].score){
            ind_melhor=j;
        }
        
    }

    for(int i =0 ; i<7 + 2*(pop_peixe); i++){
        for(int j=0; j<4; j++){
           peso_melhor[i][j] =  wcamada1[i][j][ind_melhor];
        }
    }

    for(int i =0 ; i<4; i++){
        for(int j=0; j<6; j++){
           peso2_melhor[i][j] =  wcamada2[i][j][ind_melhor];
        }
    }


    
    //CROSSOVER

    for(int k =0; k<pop_golfinho; k++){
        if(k!=ind_melhor){
            tobias[k].score = 1000;//"zero" o score
            for(int i =0 ; i<7 + 2*(pop_peixe); i++){
                for(int j=0; j<4; j++){

                    wcamada1[i][j][k] = (wcamada1[i][j][k] + peso_melhor[i][j])/2;
                    
                }
            } 
        }
        
    }

     for(int k =0; k<pop_golfinho; k++){
        if(k!=ind_melhor){
            tobias[k].score = 1000;//"zero" o score
            for(int i =0 ; i<4; i++){
                for(int j=0; j<6; j++){

                    wcamada2[i][j][k] = (wcamada2[i][j][k] + peso2_melhor[i][j])/2;
                    
                }
            } 
        }
        
    }

   
   
    
    //MUTACAO E REARRANJO DA POPULACAO
    for(int k=0; k<pop_golfinho; k++){

       // srand(time(NULL)); srand null aqui faz com q todos os individuos tenham o msmo valor d chance d mutacao e gene mutado
     
         if(k != ind_melhor){
            ad_sub_mutado = rand()%100;
            matrizpeso_mutada = rand()%100;

            if(matrizpeso_mutada<=50){//MUTAR O PESO1
                gene_mutado_i = rand()%(7 + 2*(pop_peixe));
                gene_mutado_j = rand()%4;


                if(ad_sub_mutado<=50){ //50% D CHANCE D MUTAR PRA MAIS OU PRA MENOS

                    wcamada1[gene_mutado_i][gene_mutado_j][k] = wcamada1[gene_mutado_i][gene_mutado_j][k]*(1 + percent_mutacao);

                    // for(int i =0 ; i<7 + 2*(pop_peixe); i++){
                    //     for(int j=0; j<4; j++){
                    //         wcamada1[i][j][k] = wcamada1[i][j][k]*(1 + percent_mutacao);
                            
                    //     }
                    // }
                    
                }else{


                    wcamada1[gene_mutado_i][gene_mutado_j][k] = wcamada1[gene_mutado_i][gene_mutado_j][k]*(1 - percent_mutacao);

                    // for(int i =0 ; i<7 + 2*(pop_peixe); i++){
                    //     for(int j=0; j<4; j++){
                    //         wcamada1[i][j][k] = wcamada1[i][j][k]*(1 - percent_mutacao);
                            
                    //     }
                    // }
                    
                   
                }

            }else{//MUDAR O PESO2
                gene_mutado_i = rand()%4;
                gene_mutado_j = rand()%6;

                if(ad_sub_mutado<=50){ //50% D CHANCE D MUTAR PRA MAIS OU PRA MENOS
                    wcamada2[gene_mutado_i][gene_mutado_j][k] = wcamada2[gene_mutado_i][gene_mutado_j][k]*(1 + percent_mutacao);



                    //  for(int i =0 ; i<7 + 2*(pop_peixe); i++){
                    //     for(int j=0; j<4; j++){
                    //         wcamada2[i][j][k] = wcamada2[i][j][k]*(1 + percent_mutacao);
                            
                    //     }
                    // }
                }else{

                    wcamada2[gene_mutado_i][gene_mutado_j][k] = wcamada2[gene_mutado_i][gene_mutado_j][k]*(1 - percent_mutacao);
                    //  for(int i =0 ; i<7 + 2*(pop_peixe); i++){
                    //     for(int j=0; j<4; j++){
                    //        wcamada2[i][j][k] = wcamada2[i][j][k]*(1 - percent_mutacao);
                            
                    //     }
                    // }
                }

            }

        }
         
    }



}



float t_y=0;
// funcao para processar eventos de teclado
static void key_event(GLFWwindow* window, int key, int scancode, int action, int mods){
   

    if(key==262 && action == GLFW_PRESS) pacu[0].posX += 0.1 ; // tecla para direita
    if(key==263 && action == GLFW_PRESS)  pacu[0].posX -= 0.1 ; // tecla para direita

    if(key==265 && action == GLFW_PRESS) pacu[0].posY += 0.1; // tecla para cima
    if(key==264 && action == GLFW_PRESS)pacu[0].posY -= 0.1; // tecla para baixo

    if(key==32 && action == GLFW_PRESS){
          printf("\nGeracao %d\nTaxa de mutacao %f\n", count_geracao,percent_mutacao);
         //printf("Melhor[%f]: %f\n",vetor_melhor_todos[0], vetor_melhor_todos[1] );
        // printf("angulo %f forca %f\n",vetor_theta[(int)vetor_melhor_todos[0]], vetor_forca_original[(int)vetor_melhor_todos[0]] );
        count_geracao ++;

        if(controlenovageracao==1){
            nova_geracao();
        }
    

        controlenovageracao = 0;
            
        

       

        for(int i=0; i<pop_golfinho;i++){

            tobias[i].direcUP=0;
            tobias[i].direcDOWN=0;
            tobias[i].direcRIGHT=0;
            tobias[i].direcLEFT=0;
            tobias[i].vida=1;
            tobias[i].rede=0;
            tobias[i].posX=0;
            tobias[i].posY=0;
             tobias[i].contato=0;

            tobias[i].veloc_atual=0;

            tobias[i].score=0;


        }

        
   
    }

}



 
int main(void){

    // abrindo arquivo
    FILE *arq_grafico = fopen("golfin.txt","w");
    char distancia_convertido[20];
    char indice_convertido[10];
    fprintf(arq_grafico,"0,1,2,3,4,5,6,7,8,9\n");

   
 
    // inicicializando o sistema de\ janelas
    glfwInit();

    // deixando a janela invisivel, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

 
    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(700, 700, "Minha Janela", NULL, NULL);

    
    // tornando a janela como principal 
    glfwMakeContextCurrent(window);

    // inicializando Glew (para lidar com funcoes OpenGL)
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s", glewGetErrorString(GlewInitResult));


    // GLSL para Vertex Shader
    char* vertex_code =
    "attribute vec2 position;\n"
    "uniform mat4 mat_transformation;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = mat_transformation*vec4(position, 0.0, 1.0);\n"
    "}\n";

    // GLSL para Fragment Shader
    char* fragment_code =
    "uniform vec4 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = color;\n"
    "}\n";


    // Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
    GLuint program = glCreateProgram();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);



    // Associando nosso código-fonte GLSL aos slots solicitados
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glShaderSource(fragment, 1, &fragment_code, NULL);

    // Compilando o Vertex Shader e verificando erros
    glCompileShader(vertex);

    GLint isCompiled = 0;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(vertex, infoLength, NULL, info);

        printf("Erro de compilacao no Vertex Shader.\n");
        printf("--> %s\n",&info);

    }

    

    // Compilando o Fragment Shader e verificando erros
    glCompileShader(fragment);

    isCompiled = 0;
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE){

        //descobrindo o tamanho do log de erro
        int infoLength = 512;
        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);

        //recuperando o log de erro e imprimindo na tela
        char info[infoLength];
        glGetShaderInfoLog(fragment, infoLength, NULL, info);

        printf("Erro de compilacao no Fragment Shader.\n");
        printf("--> %s\n",&info);

    }

    // Associando os programas compilado ao programa principal
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // Linkagem do programa e definindo como default
    glLinkProgram(program);
    glUseProgram(program);
 

    // Preparando dados para enviar a GPU
    
    coordenadas vertices[64];//36
    float counter = 0;
    float radius_golf = 0.03;
    float angle = 0.0;
    float x, y;

    //golfinho
    for(int i=0; i < 32; i++){
        angle += (2.0*pi)/32;
        x = cos(angle)*radius_golf;
        y = sin(angle)*radius_golf;
        vertices[i].x = x ;
        vertices[i].y = y ;
        
    }

    //peixe
    float radius_peixe = 0.01;
    for(int i=32; i < 64; i++){
        angle += (2.0*pi)/32;
        x = cos(angle)*radius_peixe;
        y = sin(angle)*radius_peixe;
        vertices[i].x = x ;
        vertices[i].y = y ;
        
    }

    float *ptr_matriz;

    float mat_transform[16] = {
        1.0f, 0.0f, 0.0f, 0.0f ,
        0.0f, 1.0f, 0.0f, 0.0f ,
        0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    };

    ptr_matriz=mat_transform;

    float *ptr_resposta;
    ptr_resposta=vetor_transicao;

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);


    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);


    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
 

    // Associando nossa janela com eventos de teclado
    glfwSetKeyCallback(window, key_event); // teclado
    
    GLint loc_color = glGetUniformLocation(program, "color");
 

    // Exibindo nossa janela
    glfwShowWindow(window);

 

    

  
    float R = 1;
    float G = 1;
    float B = 1;

    float aux;
    int controle_print1 = -1;
    float distancia_peixe;
   
   srand((unsigned int)time(NULL));
   float weight;
    //printf("\n");

    for(int i=0;i<7 + 2*(pop_peixe);i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<pop_golfinho;k++){
                weight=rand()%600;
                weight = (weight-300)/100;
                wcamada1[i][j][k]= weight;

            }

            printf("%f ",wcamada1[i][j][0] );
        }
        printf("\n");
    }

    

     for(int i=0;i<4;i++){
        for(int j=0;j<6;j++){
            for(int k=0;k<pop_golfinho;k++){
                weight=rand()%600;
                weight = (weight-300)/100;
                wcamada2[i][j][k]= weight;
            }
        }
    }

     for(int k=0;k<pop_golfinho;k++){
        printf("%d\n",k );
        for(int i=0;i<7 + 2*(pop_peixe);i++){
            for(int j=0;j<4;j++){
               printf("%f ",wcamada1[i][j][k] );

            }

             printf("\n");
        }
        printf("\n\n");
    }

    float direcX = 0;
    float direcY = 0;


    for(int i=0; i< pop_golfinho;i++){
        tobias[i].direcUP=0;
        tobias[i].direcDOWN=0;
        tobias[i].direcRIGHT=0;
        tobias[i].direcLEFT=0;
        tobias[i].vida=1;
        tobias[i].rede=0;
        tobias[i].posX=0;
        tobias[i].posY=0;
        tobias[i].contato=0;


        tobias[i].score=0;
    }

     for(int i=0; i< pop_peixe;i++){
        pacu[i].posX = 0.2;
        pacu[i].posY = 0.2 ;
    }

   // printf("melhor de todos valor%f e indice: %f\n",vetor_melhor_todos[1],vetor_melhor_todos[0] );

    
    int tempoteste = 0;
    while (!glfwWindowShouldClose(window)  ){
        

        // if(controle_print1 != count_geracao){// printar a primeira vez, dps ele printa no clique do espaço
        //     for(int j=0;j<populacao;j++){
        //         sprintf(distancia_convertido, "%.5f", vetor_cesta_acertotxt[j] );
        //         sprintf(indice_convertido, "%d", j);
        //        // fprintf(arq_grafico,"%s,",indice_convertido);
        //         fprintf(arq_grafico,"%s,",distancia_convertido);
        //          vetor_cesta_acertotxt[j]=1000;

        //     }

        //     fprintf(arq_grafico,"\n"); 
            

        //     controle_print1 = count_geracao;
        // }

      
        tempoteste = tempoteste + 1;
       
        
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.6784, 0.8471, 0.9020, 1.0);

        // enviando a matriz de transformacao para a GPU, vertex shader, variavel mat_transformation
        loc = glGetUniformLocation(program, "mat_transformation");
        melhor = 0;
        for(int j=1; j<pop_golfinho; j++){
            if(tobias[j].score>tobias[melhor].score ){
                melhor=j;
            }
        
        }
    

        for(int i=0; i<pop_golfinho; i++){
            if(tobias[i].vida>0){
          //  printf("\n\n\n\nGOLFIN %d\n\n",i );

                //tobias[i].score = 0;

                // for(int j=0; j<pop_peixe;j++){

                //     distancia_peixe = fabs(pacu[j].posX - tobias[i].posX);
                //     tobias[i].distXpeixe[j]=distancia_peixe;

                //     distancia_peixe = fabs(pacu[j].posY - tobias[i].posY);
                //     tobias[i].distYpeixe[j]=distancia_peixe;
                
                //     tobias[i].score=tobias[i].score + tobias[i].distXpeixe[j] + tobias[i].distYpeixe[j];

                    
                // }

              //  printf("Melhor %d Score do %d: %f\n",melhor,i,tobias[i].score );

                tobias[i].score = tobias[i].score + 1;

                tobias[i].vida = tobias[i].vida - 0.001;
                if((tobias[i].posX>1 + radius_golf) || (tobias[i].posX< -1 - radius_golf) ||(tobias[i].posY>1 + radius_golf) || (tobias[i].posY< -1 - radius_golf)){
                    tobias[i].vida = 0;
                   // tobias[i].score = tobias[i].score - 10000;
                }
               
               // printf("vida do %d:%f\n",i,tobias[i].vida );
               
                for(int j=0; j<pop_peixe;j++){

                    distancia_peixe = pow(fabs(pacu[j].posX - tobias[i].posX),2);
                    distancia_peixe = distancia_peixe + pow(fabs(pacu[j].posY - tobias[i].posY),2);
                    distancia_peixe = sqrt(distancia_peixe);

                    if(distancia_peixe < radius_golf && tobias[i].vida < 1){
                        printf("CONTATOOOOO\n");
                        tobias[i].vida = tobias[i].vida + 1;
                        tobias[i].contato = 1;
                        controlenovageracao = 1;
                    }
                  
                    
                }
               
                rede_neural(pop_golfinho, vetor_transicao,i,wcamada1,wcamada2,tobias[i].veloc_atual,tobias[i].direcUP,tobias[i].direcDOWN,tobias[i].direcRIGHT,tobias[i].direcLEFT,tobias[i].rede,tobias[i].vida,tobias[i].distXpeixe, tobias[i].distYpeixe);
                //rede_neural(pop_golfinho, vetor_transicao,i,wcamada1,wcamada2,testerede,testerede2,testerede,testerede2,testerede3,testerede2,testerede3);

               
                if(i==melhor){
                    printf("Vida:%f\n",tobias[i].vida );
                }
                tobias[i].direcRIGHT = vetor_transicao[1];
                tobias[i].direcLEFT = vetor_transicao[2];
                tobias[i].direcUP = vetor_transicao[3];
                tobias[i].direcDOWN = vetor_transicao[4];
                tobias[i].veloc_atual = vetor_transicao[0];
                tobias[i].rede = vetor_transicao[5];
                


                tobias[i].posX = tobias[i].posX + vetor_transicao[0]*vel_global*(vetor_transicao[1]-vetor_transicao[2]);
                tobias[i].posY = tobias[i].posY + vetor_transicao[0]*vel_global*(vetor_transicao[3]-vetor_transicao[4]);
                if(tobias[i].direcRIGHT<0.4 && tobias[i].direcLEFT<0.4){
                    tobias[i].direcRIGHT=0;
                    tobias[i].direcLEFT=0;
                 }
                // if(tobias[i].direcLEFT<0.4){
                //     tobias[i].direcLEFT=0;
                // }
                //  if(tobias[i].direcUP<0.4){
                //     tobias[i].direcUP=0;
                // }
                if(tobias[i].direcDOWN<0.4 && tobias[i].direcUP<0.4){
                    tobias[i].direcDOWN=0;
                    tobias[i].direcUP=0;
                }
                if( tobias[i].veloc_atual<0.4){
                    tobias[i].veloc_atual=0;
                }
                tobias[i].posX = tobias[i].posX +  tobias[i].veloc_atual*vel_global*(tobias[i].direcRIGHT- tobias[i].direcLEFT);
                tobias[i].posY = tobias[i].posY +  tobias[i].veloc_atual*vel_global*( tobias[i].direcUP-tobias[i].direcDOWN);

             

               // printf("direc x:%f\ndirec y:%f\n",tobias[i].posX,tobias[i].posY);

                R=0;
                G=0;
                B=1;

                if(i==melhor){
                    R=0.5;
                    G=1;
                    B=1;
                }


            

                atualiza_matriz(ptr_matriz,tobias[i].posX,tobias[i].posY);
                glUniformMatrix4fv(loc, 1, GL_TRUE, mat_transform);
                glUniform4f(loc_color, R, G, B, 1.0); 
                //desenha golfinho
                
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
                
                


            }
                 


        }

        //srand(time(NULL));
        srand((unsigned int)time(NULL));

       for(int j=0; j<pop_peixe;j++){
        int random = rand()%4;

       // pacu[j].posY = pacu[j].posY  - 0.0001;
        
        // if(random==0&& pacu[j].posY<1){


        //     //pacu[j].posX = pacu[j].posX  + 0.001;
        //     pacu[j].posY = pacu[j].posY  + 0.001;

        // }

        //  if(random==1&& pacu[j].posY>-1){
        //    // pacu[j].posX = pacu[j].posX  + 0.001;
        //     pacu[j].posY = pacu[j].posY  - 0.001;

        // }

        //  if(random==2&& pacu[j].posY<1){
        //    // pacu[j].posX = pacu[j].posX  - 0.001;
        //     pacu[j].posY = pacu[j].posY  + 0.001;

        // }

        //  if(random==3 && pacu[j].posY>-1){
        //    // pacu[j].posX = pacu[j].posX  - 0.001;
        //     pacu[j].posY = pacu[j].posY  - 0.001;

        // }
        


        R=1;
        G=0;
        B=0;
        atualiza_matriz(ptr_matriz,pacu[j].posX ,pacu[j].posY);
        //atualiza_matriz(ptr_matriz,0.5,0.5);
        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_transform);
        glUniform4f(loc_color, R, G, B, 1.0); 
        //desenha peixe
        glDrawArrays(GL_TRIANGLE_FAN, 32, 32);


       }


        
       
        glfwSwapBuffers(window);
        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);



    }

    fclose(arq_grafico);
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);

}