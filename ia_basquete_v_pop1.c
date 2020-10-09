/* para linux, instalar os pacotes libglfw3-dev mesa-common-dev libglew-dev */
/* para compilar no linux: gcc gravidadetempo.c -lglfw -lGL -lGLEW -lm -o gravidadetempo.bin */



#include <GL/glew.h>  

#define GLFW_INCLUDE_NONE
#define GLFW_FALSE   0

#include <GLFW/glfw3.h> /* verifique no seu SO onde fica o glfw3.h */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

 

// variaveis globais
typedef struct{
    float x, y;
} coordenadas;


float t_x = 0.0f; 
float t_y = 0.0f;


// funcao para processar eventos de teclado
static void key_event(GLFWwindow* window, int key, int scancode, int action, int mods){
    printf("Pressionando tecla %d\n", key);

    if(key==262) t_x += 0.01; // tecla para direita
    if(key==263) t_x -= 0.01; // tecla para direita

    if(key==265) t_y += 0.01; // tecla para cima
    if(key==264) t_y -= 0.01; // tecla para baixo

}

 
int main(void){
 
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
    "    gl_FragColor = vec4(1.0, 0.6471, 0.0, 1.0);\n"
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
    coordenadas vertices[34];
    float pi = 3.14;
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

            vertices[32].x = +0.40;
            vertices[32].y = +0.10;
            vertices[33].x = +0.55;
            vertices[33].y = +0.10;

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
    float cestax = (vertices[32].x + vertices[33].x) /2;
    float cestay = (vertices[32].y + vertices[33].y) /2;
    float bolax = (vertices[0].x + vertices[16].x)/2 ;
    float bolay = vertices[0].y ; // nao estou mais gerando a bola na posicao inicial ver linha 221 e 285
    float quinax1 = vertices[32].x;
    float quinax2 = vertices[33].x;
    float quinay1 = vertices[32].y;
    float quinay2 = vertices[33].y;//na vdd quinay1 e quinay2 tem o msmo valor pq n tem cesta inclinada


    // os valores iniciais sao guardados e os atuais sao atualizados assim como acontece com as coordenadas das figuras
    float cestax_atual, cestay_atual, bolax_atual, bolay_atual;

    //se a bola bater na lateral da cesta ela para
    float quina_acerto=0;

    // acerto eh a distancia da bola (centro da bola) da cesta (meio da cesta)
    float  acerto = fabsf(cestax - bolax) + fabsf(cestay - bolay);
    /* cesta
    float g = 10;
    float f = 5.4;
    float theta = 73;  72 ele bate na quina
    */

    

    float g = 10;
    float f = 5.4; //velocidade = esse f vezes a qntd de refreshs por s q o sistema da me da o valor da vel em m/s considerando q eh em metros o plano cartesiano
    float theta = 73;   


    float fx = cos(theta*pi/180)*f;
    float fy = sin(theta*pi/180)*f;
    double tempox = 0;
    float tempoy = 0;
 

    
    printf("tx %f\n",sin(theta*pi/180)*f );
    printf("bolay: %f bolax:%f \n\n\n\n\n",bolay,bolax );

    //pos inicial da bola
   float pos_yinicial = -0.79415;
   float pos_xinicial = -0.799995;
   //bolay = -0.79415;

    t_y =  pos_yinicial + fy*tempoy - g*tempoy*tempoy/2;
    t_x =  pos_xinicial + fx*tempox; 




    while (!glfwWindowShouldClose(window) ){
        tempox = tempox + 0.01;
        tempoy = tempoy + 0.01;

        //atualizo posicao da bola e da cesta (falta aqui as quinas atualizadas)

        cestax_atual = cestax;
        cestay_atual = cestay;
        //bolax_atual = bolax + t_x;
        bolax_atual = t_x;
        bolay_atual =  t_y;

        //distancia da bola para o centro da cesta
        float  acerto = sqrt(fabsf(cestax_atual - bolax_atual)*fabsf(cestax_atual - bolax_atual) + fabsf(cestay_atual - bolay_atual)*fabsf(cestay_atual - bolay_atual));

        

        
        /* aqui minha bola virou um quadrado entao perco qualidade de medida
        if ((quinax1 < (bolax_atual + radius) && quinax1 > (bolax_atual -radius)) || (quinax2 > (bolax_atual - radius) && quinax2 < (bolax_atual + radius)) ){
            if(quinay1 < (bolay_atual + radius) && quinay1 > (bolay_atual - radius)){
                printf("quinaaaaaaaaa\n");
                quina_acerto = 1;
            }
        }
        */

        /*
        for(int i=0; i < num_vertices; i++){
        angle += (2.0*pi)/num_vertices;
        bolax = bolax_atual + cos(angle)*radius;
        bolay = bolay_atual + sin(angle)*radius;

        if ((quinax1 < (bolax) && quinax1 > (bolax)) || (quinax2 > (bolax) && quinax2 < (bolax)) ){
            if(quinay1 < (bolay ) && quinay1 > (bolay)){
                printf("quinaaaaaaaaa\n");
                quina_acerto = 1;
            }
        }
        vertices[i].x = x ;
        vertices[i].y = y ;
        //printf("%.2f, %.2f\n",x,y);
        }
        */

        float distancia_teste1;
        float distancia_teste2;
         distancia_teste1 = sqrt(fabsf(quinax1 - bolax_atual)*fabsf(quinax1 - bolax_atual) + fabsf(quinay1 - bolay_atual)*fabsf(quinay1 - bolay_atual));
        distancia_teste2 = sqrt(fabsf(quinax2 - bolax_atual)*fabsf(quinax2 - bolax_atual) + fabsf(quinay2 - bolay_atual)*fabsf(quinay2 - bolay_atual));

        if(distancia_teste1 < radius){
            printf("quinaaaaaaaaa1\n");
               // quina_acerto = 1;
            if(bolay_atual>quinay1 ){
                if(bolax_atual>quinax1){
                    fx = (0.7*fx);
                }else{
                     fx = -(0.7*fx);
                }

                fy = (0.7*fy);
                tempoy = 0.01;
                tempox = 0.01;
                pos_yinicial = t_y;
                pos_xinicial = t_x;

            }

            if(bolay_atual<quinay1){

                if(bolax_atual>quinax1){
                    fx = (0.7*fx);
                }else{
                     fx = -(0.7*fx);
                }

               
                fy = -0.7*fy;
                tempoy = 0.01;
                tempox = 0.01;
                pos_yinicial = t_y;
                pos_xinicial = t_x;

            }


            
        }

         if(distancia_teste2 < radius){
            printf("quinaaaaaaaaa2\n");
               // quina_acerto = 1;
            if(bolay_atual>quinay2 ){
                if(bolax_atual>quinax2){
                    fx = (0.7*fx);
                }else{
                     fx = -(0.7*fx);
                }

                fy = (0.7*fy);
                tempoy = 0.01;
                tempox = 0.01;
                pos_yinicial = t_y;
                pos_xinicial = t_x;

            }

            if(bolay_atual<quinay2){

                if(bolax_atual>quinax2){
                    fx = (0.7*fx);
                }else{
                     fx = -(0.7*fx);
                }

               
                fy = (0.7*fy);
                tempoy = 0.01;
                tempox = 0.01;
                pos_yinicial = t_y;
                pos_xinicial = t_x;

            }


            
        }

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

       
        // criando a matriz de translacao
        float mat_translation_ball[16] = {
            1.0f, 0.0f, 0.0f, t_x ,
            0.0f, 1.0f, 0.0f, t_y ,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float mat_translation_basket[16] = {
            1.0f, 0.0f, 0.0f, 0.0f ,
            0.0f, 1.0f, 0.0f, 0.0f ,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

               
        printf("TEMPO %f\n",tempox );
     
        /*
        if(tempox > 10.915){
            printf("BOLAX %f  BOLAY %f\n", vertices[0].x - 0.03 + t_x, bolay_atual );
        printf("QUINAX %f QUINAY %f\n",quinax1, quinay1 );
        printf("QUINAX2 %f QUINAY2 %f\n",quinax2, quinay2 );
        printf("ACERTO %f\n",acerto );
            printf("Y: %f\nX: %f", vertices[0].x + t_x,vertices[0].y + t_y);
        }
        */

         
  

        if (vertices[16].x + t_x < 1.0 && !quina_acerto ){

            printf("vertice y:%f\n",vertices[24].y + t_y );

            if(vertices[24].y + t_y < -1  ){
                
                fy=0.7*sin(theta*pi/180)*f;
    
                
                f=fabsf(fy);        
                tempoy=0.01;
                pos_yinicial = t_y;
            }
        

            if(fabsf(fy) > 0.005){
               
                t_y =  pos_yinicial + fy*tempoy - g*tempoy*tempoy/2;   
            }   
        
            t_x =  pos_xinicial + fx*tempox; 
        
        }
 
        // enviando a matriz de transformacao para a GPU, vertex shader, variavel mat_transformation
        loc = glGetUniformLocation(program, "mat_transformation");
        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_ball);

           
        //renderizando
        glDrawArrays(GL_TRIANGLE_FAN, 0, 32);


        glUniformMatrix4fv(loc, 1, GL_TRUE, mat_translation_basket);


        glDrawArrays(GL_LINES, 32, 2);

       
         glfwSwapBuffers(window);
        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);             
  
    }
    glfwDestroyWindow(window);
 
    glfwTerminate();
    exit(EXIT_SUCCESS);
}