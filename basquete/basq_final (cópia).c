/* para linux, instalar os pacotes libglfw3-dev mesa-common-dev libglew-dev */
/* para compilar no linux: gcc basquete_cesta.c -lglfw -lGL -lGLEW -lm -o basquete_cesta.bin */



#include <GL/glew.h>  

#define GLFW_INCLUDE_NONE
#define GLFW_FALSE   0
#define speed_time 0.01
#define populacao  10
#define testetotal 100
#define posinicialX -0.9
#define posinicialY -0.9


#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "basq_final.h"

 


// variaveis globais
typedef struct{
    float x, y;
} coordenadas;


float t_x = 0.0f; 
float t_y = 0.0f;


float g = 10;

    float matriz_pos_inicial[populacao][2]; // cada linha um individuo, coluna 0 = x coluna 1 = y
    float matriz_tempo[populacao][2];
    float matriz_forca_atual[populacao][2];
    float matriz_cesta_atual[populacao][2];
    float matriz_bola_atual[populacao][2];
    float matriz_translado[populacao][2];
    float matriz_translado_parcial[populacao][2];
    float matriz_quina_acerto[populacao][1];
    float matriz_resultado[4][testetotal]; // linhas: 1forca 2theta 3cestax 4cestay colunas: cestas em posicoes diferentes
    float vetor_forca_original[populacao];
    float vetor_theta[populacao];
    float vetor_cesta_acerto[populacao];
    float vetor_distfinal[populacao];
    float vetor_transldado_cesta[2];
    float vetor_cesta_acertotxt[populacao];
    float vetor_melhor_todos[2]; // coluna 0 o indice coluna 1 o valor
    float distanciax;
    float distanciay;
    float subindo[populacao];
    float bolaviva[populacao];
    float pi = 3.14;
    float percent_mutacao = 0.02;
    int count_geracao = 1;
    int predacao = 0;
    int count_genocidios = 0;
    float tempoteste = 0.00;
    int stop = 0;
    int treinos = 0;




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



 
int main(void){

  
    

    // abrindo arquivo
    FILE *arq_grafico = fopen("graffinal.txt","w");
    char distancia_convertido[20];
    char indice_convertido[10];
    fprintf(arq_grafico,"0,1,2,3,4,5,6,7,8,9\n");

    FILE *arq_result = fopen("resultados8.txt","w");
    char forca_convertido[20];
    char theta_convertido[20];
    char tx_convertido[20];
    char ty_convertido[20];
    fprintf(arq_result,"0,1,2,3\n");



  


 
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
    int num_vertices = 32;
    coordenadas vertices[52];//36
   
    float counter = 0;
    float radius = 0.03;
    float angle = 0.0;
    float x, y;
    for(int i=0; i < num_vertices; i++){
        angle += (2.0*pi)/num_vertices;
        x = cos(angle)*radius;
        y = sin(angle)*radius;
        vertices[i].x = x ;
        vertices[i].y = y ;
        //printf("%.2f, %.2f\n",x,y);
    }
    printf("vertice 0 x %f y %f", vertices[0].x, vertices[0].y);

/*
            //cesta
            vertices[32].x = +0.40;
            vertices[32].y = +0.10;
            vertices[33].x = +0.55;
            vertices[33].y = +0.10;

            //tabela
            vertices[34].x = +0.56;
            vertices[34].y = +0.07;
            vertices[35].x = +0.56;
            vertices[35].y = +0.27;

            float redey = 0.06;
            //rede
            vertices[36].x = +0.41;
            vertices[36].y = +0.10;
            vertices[37].x = +0.41;
            vertices[37].y = -redey;

            vertices[38].x = +0.54;
            vertices[38].y = +0.10;
            vertices[39].x = +0.54;
            vertices[39].y = -redey;

            vertices[40].x = +0.41;
            vertices[40].y = +0.10;
            vertices[41].x = +0.54;
            vertices[41].y = -redey;

            vertices[42].x = +0.54;
            vertices[42].y = +0.10;
            vertices[43].x = +0.41;
            vertices[43].y = -redey;

            vertices[44].x = +0.475;
            vertices[44].y = +0.10;
            vertices[45].x = +0.41;
            vertices[45].y = +(0.1-redey)/2;

            vertices[46].x = +0.41;
            vertices[46].y = +(0.1-redey)/2;
            vertices[47].x = +0.475;
            vertices[47].y = -redey + 0.02;

            vertices[48].x = +0.475;
            vertices[48].y = -redey + 0.02;
            vertices[49].x = +0.54;
            vertices[49].y = +(0.1-redey)/2;

            vertices[50].x = +0.54;
            vertices[50].y = +(0.1-redey)/2;
            vertices[51].x = +0.475;
            vertices[51].y = +0.10;
            */
            //cesta
            vertices[32].x = -0.075;
            vertices[32].y = +0.00;
            vertices[33].x = +0.075;
            vertices[33].y = +0.00;

            //tabela
            vertices[34].x = +0.085;
            vertices[34].y = -0.01;
            vertices[35].x = +0.085;
            vertices[35].y = +0.19;

            float redey = 0.15;
            //rede
            vertices[36].x = -0.065;//41
            vertices[36].y = +0.00;
            vertices[37].x = -0.065;
            vertices[37].y = -redey;

            vertices[38].x = +0.065;//54
            vertices[38].y = +0.00;
            vertices[39].x = +0.065;
            vertices[39].y = -redey;

            vertices[40].x = -0.065;
            vertices[40].y = +0.00;
            vertices[41].x = +0.065;
            vertices[41].y = -redey;

            vertices[42].x = +0.065;
            vertices[42].y = +0.00;
            vertices[43].x = -0.065;
            vertices[43].y = -redey;

            vertices[44].x = +0.00;
            vertices[44].y = +0.00;
            vertices[45].x = -0.065;
            vertices[45].y = -redey/2;

            vertices[46].x = -0.065;
            vertices[46].y = -redey/2;
            vertices[47].x = +0.00;
            vertices[47].y = -redey + 0.02;

            vertices[48].x = +0.00;
            vertices[48].y = -redey + 0.02;
            vertices[49].x = +0.065;
            vertices[49].y = -redey/2;

            vertices[50].x = +0.065;
            vertices[50].y = -redey/2;
            vertices[51].x = +0.00;
            vertices[51].y = +0.00;

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

        // Exibindo nossa janela
        glfwShowWindow(window);
    

    //acho posicao do meio da cesta e do meio da bola
    float cestax = (vertices[32].x + vertices[33].x) /2  + vetor_transldado_cesta[0];
    float cestay = (vertices[32].y + vertices[33].y) /2  + vetor_transldado_cesta[1];
    float bolax = (vertices[0].x + vertices[16].x)/2 ;
    float bolay = vertices[0].y ; // nao estou mais gerando a bola na posicao inicial ver linha 221 e 285
    float quinax1 = vertices[32].x + vetor_transldado_cesta[0];
    float quinax2 = vertices[33].x + vetor_transldado_cesta[0];
    float quinay1 = vertices[32].y + vetor_transldado_cesta[1];
    float quinay2 = vertices[33].y + vetor_transldado_cesta[1];//na vdd quinay1 e quinay2 tem o msmo valor pq n tem cesta inclinada
    float tabelax1 =vertices[34].x + vetor_transldado_cesta[0]; //ponto de baixo da tabela
    float tabelay1 =vertices[34].y + vetor_transldado_cesta[1];
    float tabelax2 =vertices[35].x + vetor_transldado_cesta[0];//ponto de cima da tabela
    float tabelay2 =vertices[35].y + vetor_transldado_cesta[1];


    // os valores iniciais sao guardados e os atuais sao atualizados assim como acontece com as coordenadas das figuras
   // float cestax_atual, cestay_atual, bolax_atual, bolay_atual;

    //se a bola bater na lateral da cesta ela para
  //  float quina_acerto=0;

    // acerto eh a distancia da bola (centro da bola) da cesta (meio da cesta)
   // float  acerto = fabsf(cestax - bolax) + fabsf(cestay - bolay);
    /* cesta
    float g = 10;
    float f = 5.4;
    float vetor_theta = 73;  72 ele bate na quina
    */

    
    GLint loc_color = glGetUniformLocation(program, "color");

    srand((unsigned int)time(NULL));
    float random_theta = 90;
    float random_forca = 10;

    //inicia valores aleatorios de angulo e forca
    for(int j=0;j<populacao;j++){

        vetor_theta[j]= ((float)rand()/(float)(RAND_MAX)) * random_theta;
        vetor_forca_original[j]= ((float)rand()/(float)(RAND_MAX)) * random_forca;
        

    }


    //inicializar valores dos vetores

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

        vetor_cesta_acerto[j]=1000;
        vetor_cesta_acertotxt[j]=1000;
        vetor_distfinal[j]=1000;
       

    }


  

    float R = 0;
    float G = 0;
    float B = 1;

    float aux;
    int controle_print1 = -1;
   

    vetor_melhor_todos[0]=-10; 
    vetor_melhor_todos[1]=1000000;
    vetor_transldado_cesta[0] = 0;
    vetor_transldado_cesta[1] = 0;

   // printf("melhor de todos valor%f e indice: %f\n",vetor_melhor_todos[1],vetor_melhor_todos[0] );

    

    while (!glfwWindowShouldClose(window) && !stop ){



        if(controle_print1 != count_geracao){// printar a primeira vez, dps ele printa no clique do espaço
            for(int j=0;j<populacao;j++){
                sprintf(distancia_convertido, "%.5f", vetor_cesta_acertotxt[j] );
                sprintf(indice_convertido, "%d", j);
               // fprintf(arq_grafico,"%s,",indice_convertido);
                fprintf(arq_grafico,"%s,",distancia_convertido);
                 vetor_cesta_acertotxt[j]=1000;

            }

            fprintf(arq_grafico,"\n"); 
            

            controle_print1 = count_geracao;
        }

       // printf("TRANSLADO EM X DA BOLA 0: %f  TRANSLADO EM Y DA BOLA 0: %f\n",matriz_translado[0][0],matriz_translado[0][1] );
        tempoteste = tempoteste + 0.001;

        


        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        //glUniform4f(loc_color, R, G, B, 1.0); // modificando a cor do objeto!
        //glDrawArrays(GL_LINE_LOOP, 0, 28);

        // enviando a matriz de transformacao para a GPU, vertex shader, variavel mat_transformation
        loc = glGetUniformLocation(program, "mat_transformation");

        cestax = (vertices[32].x + vertices[33].x) /2  + vetor_transldado_cesta[0];
        cestay = (vertices[32].y + vertices[33].y) /2  + vetor_transldado_cesta[1];
        quinax1 = vertices[32].x + vetor_transldado_cesta[0];
        quinax2 = vertices[33].x + vetor_transldado_cesta[0];
        quinay1 = vertices[32].y + vetor_transldado_cesta[1];
        quinay2 = vertices[33].y + vetor_transldado_cesta[1];//na vdd quinay1 e quinay2 tem o msmo valor pq n tem cesta inclinada
        tabelax1 =vertices[34].x + vetor_transldado_cesta[0]; //ponto de baixo da tabela
        tabelay1 =vertices[34].y + vetor_transldado_cesta[1];
        tabelax2 =vertices[35].x + vetor_transldado_cesta[0];//ponto de cima da tabela
        tabelay2 =vertices[35].y + vetor_transldado_cesta[1];
        

        for(int j = 0; j<populacao; j++){ 
  

            //atualizo o tempo
            matriz_tempo[j][0]= matriz_tempo[j][0] + speed_time;  //tempo x
            matriz_tempo[j][1]= matriz_tempo[j][1] + speed_time;  //tempo y

            //translado parcial eh o quanto ele movimnenta nessa iteracao
            matriz_translado_parcial[j][0] =  matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0]- matriz_translado[j][0];
            matriz_translado_parcial[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2 - matriz_translado[j][1];
          // printf("parcial x %f\n",matriz_translado_parcial[j][1] );

            //atualizo as posicoes da bola e da cesta
            matriz_cesta_atual[j][0]=cestax;
            matriz_cesta_atual[j][1]=cestay;
            matriz_bola_atual[j][0]=matriz_translado[j][0];
            matriz_bola_atual[j][1]=matriz_translado[j][1];

            //calculo a distancia entre a bola e o centro da cesta
            float distanciax = fabsf(matriz_cesta_atual[j][0]-matriz_bola_atual[j][0]);
            float distanciay = fabsf(matriz_cesta_atual[j][1]-matriz_bola_atual[j][1]);
            vetor_cesta_acerto[j]= sqrt(distanciax*distanciax + distanciay*distanciay);

            if(vetor_distfinal[j]>vetor_cesta_acerto[j]){
                vetor_distfinal[j]=vetor_cesta_acerto[j];
            }
           
            if(vetor_cesta_acerto[j]<vetor_cesta_acertotxt[j]){
                vetor_cesta_acertotxt[j] = vetor_cesta_acerto[j]; //salvo os dados pra gerar o arquivo txt
               
            }

            //atualizo melhor de todos
            if(vetor_cesta_acerto[j]<vetor_melhor_todos[1] && bolaviva[j] > 0 && subindo[j]<0){
               // if(vetor_cesta_acerto[j]<vetor_melhor_todos[1] ){
                vetor_melhor_todos[0] = j;
                vetor_melhor_todos[1] = vetor_cesta_acerto[j];
                predacao = 0;
                count_genocidios=0;
            }
            //vetor_melhor_todos[1] = vetor_cesta_acerto[ vetor_melhor_todos[0];
            
            //calculo colisao com as quinas aqui se eu mudar a pos da cesta a pos das quinas nao muda
            float distancia_quina1;
            float distancia_quina2;
            /*
            distancia_quina1 = fabsf(quinax1 - matriz_bola_atual[j][0] + matriz_translado_parcial[j][0]) * fabsf(quinax1 - matriz_bola_atual[j][0] + matriz_translado_parcial[j][0]);
            distancia_quina1 = distancia_quina1 + fabsf(quinay1 - matriz_bola_atual[j][1]+ matriz_translado_parcial[j][1] )*fabsf(quinay1 - matriz_bola_atual[j][1]+ matriz_translado_parcial[j][1] );
            distancia_quina1 = sqrt(distancia_quina1);

            distancia_quina2 = fabsf(quinax2 - matriz_bola_atual[j][0] + matriz_translado_parcial[j][0]) * fabsf(quinax2 - matriz_bola_atual[j][0] + matriz_translado_parcial[j][0]);
            distancia_quina2 = distancia_quina2 + fabsf(quinay2 - matriz_bola_atual[j][1]+ matriz_translado_parcial[j][1] )*fabsf(quinay2 - matriz_bola_atual[j][1]+ matriz_translado_parcial[j][1] );
            distancia_quina2 = sqrt(distancia_quina2);
            */
           
            distancia_quina1 = fabsf(quinax1 - matriz_bola_atual[j][0]) * fabsf(quinax1 - matriz_bola_atual[j][0] );
            distancia_quina1 = distancia_quina1 + fabsf(quinay1 - matriz_bola_atual[j][1] )*fabsf(quinay1 - matriz_bola_atual[j][1] );
            distancia_quina1 = sqrt(distancia_quina1);

            distancia_quina2 = fabsf(quinax2 - matriz_bola_atual[j][0] ) * fabsf(quinax2 - matriz_bola_atual[j][0] );
            distancia_quina2 = distancia_quina2 + fabsf(quinay2 - matriz_bola_atual[j][1] )*fabsf(quinay2 - matriz_bola_atual[j][1] );
            distancia_quina2 = sqrt(distancia_quina2);




            if(distancia_quina1 < radius){

                if(matriz_quina_acerto[j][0]==1){
                    matriz_quina_acerto[j][0]=11;
                }
                if(matriz_quina_acerto[j][0]==0){
                    matriz_quina_acerto[j][0]= 1;
                    
                   
                }
                
            }else{

                if(distancia_quina2 < radius){
                    if(matriz_quina_acerto[j][0]==2){
                    matriz_quina_acerto[j][0]=22;
                    }
                    if(matriz_quina_acerto[j][0]==0){
                        matriz_quina_acerto[j][0]= 2;
                    }

                    

                }else{

                    matriz_quina_acerto[j][0]= 0;

                }


            }
           
            //caso haja colisao ponho o quique da bola
            if(distancia_quina1  < radius && matriz_quina_acerto[j][0] == 1){

                if(matriz_bola_atual[j][1] + matriz_translado_parcial[j][1]>quinay1){ // se a bola ta pra cima da quina entao o quique eh pra cima

                    if(matriz_bola_atual[j][0] + matriz_translado_parcial[j][0]>quinax1){ // se a bola ta pra esquerda da quina entao o quique eh pra esquerda caso contrario pra direita
                        matriz_forca_atual[j][0] = 0.7*matriz_forca_atual[j][0];
                    }else{
                        matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    }

                    matriz_forca_atual[j][1]= 0.7*matriz_forca_atual[j][1];
                    matriz_tempo[j][0]=speed_time;
                    matriz_tempo[j][1]=speed_time;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];


                }

                if(matriz_bola_atual[j][1] + matriz_translado_parcial[j][1]<quinay1){ // se a bola ta pra baixo da quina entao o quique eh pra baixo

                    if(matriz_bola_atual[j][0] + matriz_translado_parcial[j][0]>quinax1){ // mesma analise acima
                        matriz_forca_atual[j][0] = 0.7*matriz_forca_atual[j][0];
                    }else{
                        matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    }

                    matriz_forca_atual[j][1]= -0.7*matriz_forca_atual[j][1];
                    matriz_tempo[j][0]=speed_time;
                    matriz_tempo[j][1]=speed_time;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];


                }

            }


            //repito pra quina 2
            if(distancia_quina2 < radius && matriz_quina_acerto[j][0] == 2){

                if(matriz_bola_atual[j][1]>quinay1){ // se a bola ta pra cima da quina entao o quique eh pra cima

                    if(matriz_bola_atual[j][0]>quinax1){ // se a bola ta pra esquerda da quina entao o quique eh pra esquerda caso contrario pra direita
                        matriz_forca_atual[j][0] = 0.7*matriz_forca_atual[j][0];
                    }else{
                        matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    }

                    matriz_forca_atual[j][1]= 0.7*matriz_forca_atual[j][1];
                    matriz_tempo[j][0]=speed_time;
                    matriz_tempo[j][1]=speed_time;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];


                }

                if(matriz_bola_atual[j][1]<quinay1){ // se a bola ta pra baixo da quina entao o quique eh pra baixo

                    if(matriz_bola_atual[j][0]>quinax1){ // mesma analise acima
                        matriz_forca_atual[j][0] = 0.7*matriz_forca_atual[j][0];
                    }else{
                        matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    }

                    matriz_forca_atual[j][1]= -0.7*matriz_forca_atual[j][1];
                    matriz_tempo[j][0]=speed_time;
                    matriz_tempo[j][1]=speed_time;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];

                }

            }


            //colisao com tabela
            //vejo se ta na altura da tabela
            if((matriz_bola_atual[j][1] + matriz_translado_parcial[j][1] > tabelay1) && (matriz_bola_atual[j][1] + matriz_translado_parcial[j][1] < tabelay2)){


                if(matriz_bola_atual[j][0] + matriz_translado_parcial[j][0] + radius > tabelax1 && matriz_bola_atual[j][0] + matriz_translado_parcial[j][0] - radius  < tabelax1){

                    matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    if(subindo[j]<0){
                         matriz_forca_atual[j][1]= -0.7*matriz_forca_atual[j][1];
                    }else{
                        matriz_forca_atual[j][1]= 0.7*matriz_forca_atual[j][1];
                    }
                    
                    matriz_tempo[j][0]=speed_time;
                    matriz_tempo[j][1]=speed_time;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];


                }

            }







            //acrescento o movimento em x e y e o possivel quique com o chao
            if(matriz_bola_atual[j][0] - radius < 1.0){

                if(matriz_bola_atual[j][1] - radius < -1.0){ //quique
                   
                   if(matriz_forca_atual[j][1]<0){
                    matriz_forca_atual[j][1] = -0.7*matriz_forca_atual[j][1];
                }else{
                    matriz_forca_atual[j][1] = 0.7*matriz_forca_atual[j][1];
                }
                    

                   //vetor_forca_original[j]= fabsf(matriz_forca_atual[j][1]);
                    matriz_tempo[j][1]=speed_time;
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];
                }

                if(fabsf(matriz_forca_atual[j][1]) > 0.005){ // se a força for grande o suficiente ele movimenta em y
                    
                    aux = matriz_translado[j][1];
                    matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;
                   // matriz_translado_parcial[j][1] = matriz_translado[j][1] - matriz_pos_inicial[j][1]
                    subindo[j] = matriz_translado[j][1]- aux;// posicao menos a posicao anterior se der negativo ele ta descendo
                                                          // guardar essa info para nao aceitar cestas de baixo pra cima

                    //se a bola ta subindo e ta embaixo do aro ela eh invalida
                    if(subindo[j]>0 && (matriz_translado[j][0]>quinax1 && matriz_translado[j][0]<quinax2) && matriz_translado[j][1]<quinay1){
                        bolaviva[j]=-1;
                    }
                        
                    

                }

                //movimento em x
                matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];


            }

           





            


            for(int j = 0; j<populacao; j++){
                float mat_translation_ball[16] = {
                    1.0f, 0.0f, 0.0f, matriz_translado[j][0] ,
                    0.0f, 1.0f, 0.0f, matriz_translado[j][1] ,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                };
 

                if(vetor_melhor_todos[0]==j){
                    R=0.00;
                    G=0.30;
                    B=1.00;

                    /*
                    // enviando a matriz de transformacao para a GPU, vertex shader, variavel mat_transformation
                    loc = glGetUniformLocation(program, "mat_transformation");
                    glUniform4f(loc_color, R, G, B, 1.0); 
                    glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_ball);
                    */

               
                    //renderizando
                    //loc = glGetUniformLocation(program, "mat_transformation");
                    glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_ball);
                    glUniform4f(loc_color, R, G, B, 1.0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
                 
                }else{
                    
                    R=1.0;
                    G=0.614;
                    B=0.0;
                }
                
                
                //renderizando
                glUniform4f(loc_color, R, G, B, 1.0); 
                glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_ball);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 32);
                

            }


        }

            
       
        // criando a matriz de translacao
            
        float mat_translation_basket[16] = {
            1.0f, 0.0f, 0.0f,  vetor_transldado_cesta[0] ,
            0.0f, 1.0f, 0.0f,  vetor_transldado_cesta[1] ,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };


        R=1;
        G=1;
        B=1;
        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_basket);
        glUniform4f(loc_color, R, G, B, 1.0); 
        //desenha cesta
        glDrawArrays(GL_LINES, 32, 2);

        R=1;
        G=0;
        B=0;
        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_basket);
        glUniform4f(loc_color, R, G, B, 1.0); 
        //desenha tabela
        glDrawArrays(GL_LINES, 34, 2);


        R=0.502;
        G=0.502;
        B=0.502;


        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_basket);
        glUniform4f(loc_color, R, G, B, 1.0); 
        //desenha tabela
        glDrawArrays(GL_LINES, 36, 16);

        glfwSwapBuffers(window);
        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);


        //reseta o lancamento sozinho
        if(tempoteste > 0.2){
        
            resetapop();

        }
  
 

        //muda a cesta de lugar caso ja tenha sido cesta
        if( vetor_melhor_todos[1]<0.015){//count_genocidios > 1 &&

  
             
            //salvo os dados deste treino
            matriz_resultado[0][treinos] = vetor_forca_original[(int)vetor_melhor_todos[0]];
            matriz_resultado[1][treinos] = vetor_theta[(int)vetor_melhor_todos[0]];
            matriz_resultado[2][treinos] = vetor_transldado_cesta[0];
            matriz_resultado[3][treinos] = vetor_transldado_cesta[1];

             
            //converto os dados para char
            sprintf(forca_convertido, "%.5f", matriz_resultado[0][treinos]);
            sprintf(theta_convertido, "%.5f", matriz_resultado[1][treinos]);
            sprintf(tx_convertido, "%.5f", matriz_resultado[2][treinos]);
            sprintf(ty_convertido, "%.5f", matriz_resultado[3][treinos]);

            //escrevo no arquivo
            fprintf(arq_result,"%s,%s,%s,%s\n",forca_convertido,theta_convertido,tx_convertido,ty_convertido);
    

        

            float random = 1.6;
            
            //gera um nmero aleatorio entre 0 e rand max, divide por rand max, nmero entre 0 e 1
            //vezes o 1.6 (nmero entre 0 e 1.6), -0.8 nmero entre -0.8 e 0.8
            vetor_transldado_cesta[0]= (((float)rand()/(float)(RAND_MAX)) * random) - 0.8; 
            vetor_transldado_cesta[1]= (((float)rand()/(float)(RAND_MAX)) * random) - 0.8; 

            //resetapop();
            tempoteste=0;
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

            treinos += 1;

            if(treinos > testetotal){
                stop = 1;
            }

               

        }


    }

    fclose(arq_grafico);
    fclose(arq_result);
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);

}