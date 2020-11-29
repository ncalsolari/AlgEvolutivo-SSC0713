/* para linux, instalar os pacotes libglfw3-dev mesa-common-dev libglew-dev */
/* para compilar no linux: gcc golfinho.c -lglfw -lGL -lGLEW -lm -o golfinho.bin */



#include <GL/glew.h>  

#define GLFW_INCLUDE_NONE
#define GLFW_FALSE   0
#define speed_time 0.005
#define populacao  10


#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

 


// variaveis globais
typedef struct{
    float x, y;
} coordenadas;

typedef struct{
    float x, y;
} golfinho;

typedef struct{
    float x, y;
} peixe;


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
    float vetor_forca_original[populacao];
    float vetor_theta[populacao];
    float vetor_cesta_acerto[populacao];
    float vetor_cesta_acertotxt[populacao];
    float vetor_melhor_todos[2]; // coluna 0 o indice coluna 1 o valor
    float distanciax;
    float distanciay;
    float subindo[populacao];
    float pi = 3.14;
    float percent_mutacao = 0.02;
    int count_geracao = 1;
    int predacao = 0;


static void atualiza_matriz(float *pont_m,float t_x,float t_y,float s_x,float s_y){



    float matrix_transform = glm.mat4(1.0)// instanciando uma matriz identidade
    matrix_transform = glm.translate(matrix_transform, glm.vec3(t_x, t_y, 0));
    matrix_transform = glm.scale(matrix_transform, glm.vec3(s_x, s_y, 1));

    int count = 0;

    for(int j=0; j<4; j++){
        for(int i=0; i<4; i++){


          *(pont_m+count)=matrix_transform[i][j] ;

          count++;

        }
    }


}


// funcao para processar eventos de teclado
static void key_event(GLFWwindow* window, int key, int scancode, int action, int mods){
   

    if(key==262 && action == GLFW_PRESS) percent_mutacao += 0.01; // tecla para direita
    if(key==263 && action == GLFW_PRESS) percent_mutacao -= 0.01; // tecla para direita

    if(key==265) t_y += 0.1; // tecla para cima
    if(key==264) t_y -= 0.1; // tecla para baixo

    if(key==32 && action == GLFW_PRESS){
        printf("\nGeracao %d\nTaxa de mutacao %f\nDistancia do melhor %f\n", count_geracao,percent_mutacao,vetor_melhor_todos[1]);
         //printf("Melhor[%f]: %f\n",vetor_melhor_todos[0], vetor_melhor_todos[1] );
        // printf("angulo %f forca %f\n",vetor_theta[(int)vetor_melhor_todos[0]], vetor_forca_original[(int)vetor_melhor_todos[0]] );
        count_geracao ++;
        
    

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
        

       

        for(int j=0; j<populacao;j++){

            matriz_forca_atual[j][0]= cos(vetor_theta[j]*pi/180)*vetor_forca_original[j];
            matriz_forca_atual[j][1]= sin(vetor_theta[j]*pi/180)*vetor_forca_original[j];

            matriz_tempo[j][0]=speed_time;
            matriz_tempo[j][1]=speed_time;

            matriz_pos_inicial[j][0]= -0.799995;
            matriz_pos_inicial[j][1]= -0.79415;

            matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];
            matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;

            subindo[j]=1;

            matriz_quina_acerto[j][0]=0;

            
           

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
    float radius = 0.03;
    float angle = 0.0;
    float x, y;

    //golfinho
    for(int i=0; i < 32; i++){
        angle += (2.0*pi)/num_vertices;
        x = cos(angle)*radius;
        y = sin(angle)*radius;
        vertices[i].x = x ;
        vertices[i].y = y ;
        
    }

    //peixe
    radius = 0.01
    for(int i=32; i < 64; i++){
        angle += (2.0*pi)/num_vertices;
        x = cos(angle)*radius;
        y = sin(angle)*radius;
        vertices[i].x = x ;
        vertices[i].y = y ;
        
    }


    float matriz_identidade = glm.mat4(1.0);

    float transladox = 0.2;
    float transladoy = 0.5;
    float matriz_translacao = glm.mat4(1.0); // inicializa uma matriz identidade
    //matriz_translacao = glm.translate(matriz_translacao, glm.vec3(t_x, t_y, 0));  //aplica translacao com base nos offsets  

    float escalax = 1;
    float escalay = 1;
    float matriz_escala = glm.mat4(1.0); //inicializa uma matriz identidade
    //matriz_escala = glm.scale(matriz_escala, glm.vec3(s_x, s_y, 1));  //aplica escala

    float angulo = 60.0;
    angulo = math.radians(angulo);
    float r_x = 0.0;
    float r_y = 0.0;
    float r_z = 0.0;
    // matriz_rotacao = glm.mat4(1.0) // inicializa uma matriz identidade
    // matriz_rotacao = glm.rotate(matriz_rotacao, angulo, glm.vec3(r_x, r_y, r_z));// aplica rotacao
    

    float matrix_transform = glm.mat4(1.0);// instanciando uma matriz identidade
    matrix_transform = glm.rotate(matrix_transform, angulo, glm.vec3(r_x, r_y, r_z));
    matrix_transform = glm.translate(matrix_transform, glm.vec3(transladox, transladoy, 0));
    matrix_transform = glm.scale(matrix_transform, glm.vec3(escalax, escalay, 1));

    float vetor_matrix_transform[16];
    int count = 0;
    float *ptr;
    ptr = vetor_matrix_transform;

    for(int j=0; j<4; j++){
        for(int i=0; i<4; i++){


          vetor_matrix_transform[count]=matrix_transform[i][j] 

          count++;

        }
    }



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

 

    GLint loc_color = glGetUniformLocation(program, "color");

  
    float R = 0;
    float G = 0;
    float B = 1;

    float aux;
    int controle_print1 = -1;
   

    vetor_melhor_todos[0]=-10;
    vetor_melhor_todos[1]=1000000;

   // printf("melhor de todos valor%f e indice: %f\n",vetor_melhor_todos[1],vetor_melhor_todos[0] );

    
    float tempoteste = 0.00;
    while (!glfwWindowShouldClose(window) ){

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

       // printf("TRANSLADO EM X DA BOLA 0: %f  TRANSLADO EM Y DA BOLA 0: %f\n",matriz_translado[0][0],matriz_translado[0][1] );
        tempoteste = tempoteste + 0.001;
      //printf("tempo %f\n",tempoteste );

        


        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        //glUniform4f(loc_color, R, G, B, 1.0); // modificando a cor do objeto!
        //glDrawArrays(GL_LINE_LOOP, 0, 28);

        // enviando a matriz de transformacao para a GPU, vertex shader, variavel mat_transformation
        loc = glGetUniformLocation(program, "mat_transformation");

        

           
       
      
            
        R=0;
        G=0;
        B=1;
        transladox=0;
        transladoy=0;
        atualiza_matriz(ptr,0,0,1,1);
        glUniformMatrix4fv(loc, 1, GL_TRUE, matrix_transform);
        glUniform4f(loc_color, R, G, B, 1.0); 
        //desenha golfinho
        glDrawArrays(GL_LINES, 0, 32);

        R=1;
        G=0;
        B=0;
        atualiza_matriz(ptr,0.5,0.5,1,1);
        glUniformMatrix4fv(loc, 1, GL_TRUE, matrix_transform);
        glUniform4f(loc_color, R, G, B, 1.0); 
        //desenha peixe
        glDrawArrays(GL_LINES, 32, 32);

       
        glfwSwapBuffers(window);
        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);



    }

    fclose(arq_grafico);
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);

}