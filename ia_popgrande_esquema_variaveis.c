for(int i = 0; i<1000; i++){

        for(int j = 0; j<2; j++){

            //atualizo o tempo
            matriz_tempo[j][0]=matriz_tempo[j][0] + 0.01;  //tempo x
            matriz_tempo[j][1]=matriz_tempo[j][1] + 0.01;  //tempo y

            //atualizo as posicoes da bola e da cesta
            matriz_cesta_atual[j][0]=cestax;
            matriz_cesta_atual[j][1]=cestay;
            matriz_bola_atual[j][0]=matriz_translado[j][0];
            matriz_bola_atual[j][1]=matriz_translado[j][1];

            //calculo a distancia entre a bola e o centro da cesta
            float distanciax = fabsf(matriz_cesta_atual[j][0]-matriz_bola_atual[j][0]);
            float distanciay = fabsf(matriz_cesta_atual[j][1]-matriz_bola_atual[j][1]);
            vetor_cesta_acerto[j]= sqrt(distanciax*distanciax + distanciay*distanciay);

            //atualizo melhor de todos
            if(vetor_cesta_acerto[j]>vetor_melhor_todos[1]){
                vetor_melhor_todos[0] = j;
                vetor_melhor_todos[1] = vetor_cesta_acerto[j];
            }


            //calculo colisao com as quinas aqui se eu mudar a pos da cesta a pos das quinas nao muda
            float distancia_quina1;
            float distancia_quina2;
            distancia_quina1 = fabsf(quinax1 - matriz_bola_atual[j][0]) * fabsf(quinax1 - matriz_bola_atual[j][0]);
            distancia_quina1 = distancia_quina1 + fabsf(quinay1 - matriz_bola_atual[j][1])*fabsf(quinay1 - matriz_bola_atual[j][1]);
            distancia_quina1 = sqrt(distancia_quina1);

            distancia_quina2 = fabsf(quinax2 - matriz_bola_atual[j][0]) * fabsf(quinax2 - matriz_bola_atual[j][0]);
            distancia_quina2 = distancia_quina2 + fabsf(quinay2 - matriz_bola_atual[j][1])*fabsf(quinay2 - matriz_bola_atual[j][1]);
            distancia_quina2 = sqrt(distancia_quina2);

            //caso haja colisao ponho o quique da bola
            if(distancia_quina1 < radius){

                if(matriz_bola_atual[j][1]>quinay1){ // se a bola ta pra cima da quina entao o quique eh pra cima

                    if(matriz_bola_atual[j][0]>quinax1){ // se a bola ta pra esquerda da quina entao o quique eh pra esquerda caso contrario pra direita
                        matriz_forca_atual[j][0] = 0.7*matriz_forca_atual[j][0];
                    }else{
                        matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    }

                    matriz_forca_atual[j][1]= 0.7*matriz_forca_atual[j][1];
                    matriz_tempo[j][0]=0.01;
                    matriz_tempo[j][1]=0.01;
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
                    matriz_tempo[j][0]=0.01;
                    matriz_tempo[j][1]=0.01;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];


                }

            }


            //repito pra quina 2
            if(distancia_quina2 < radius){

                if(matriz_bola_atual[j][1]>quinay1){ // se a bola ta pra cima da quina entao o quique eh pra cima

                    if(matriz_bola_atual[j][0]>quinax1){ // se a bola ta pra esquerda da quina entao o quique eh pra esquerda caso contrario pra direita
                        matriz_forca_atual[j][0] = 0.7*matriz_forca_atual[j][0];
                    }else{
                        matriz_forca_atual[j][0] = -0.7*matriz_forca_atual[j][0];
                    }

                    matriz_forca_atual[j][1]= 0.7*matriz_forca_atual[j][1];
                    matriz_tempo[j][0]=0.01;
                    matriz_tempo[j][1]=0.01;
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
                    matriz_tempo[j][0]=0.01;
                    matriz_tempo[j][1]=0.01;
                    matriz_pos_inicial[j][0]= matriz_translado[j][0];
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];


                }



            }






            //acrescento o movimento em x e y e o possivel quique com o chao
            if(matriz_bola_atual[j][0] - radius + matriz_translado[j][0] < 1.0){

                if(matriz_bola_atual[j][1] - radius + matriz_translado[j][1] < -1.0){ //quique

                    matriz_forca_atual[j][1] = 0.7*matriz_forca_atual[j][1];

                    vetor_forca_original[j]= fabsf(matriz_forca_atual[j][1]);
                    matriz_tempo[j][1]=0.01;
                    matriz_pos_inicial[j][1]= matriz_translado[j][1];
                }

                if(fabsf(matriz_forca_atual[j][1]) > 0.005){ // se a força for grande o suficiente ele movimenta em y

                   matriz_translado[j][1] = matriz_pos_inicial[j][1] + matriz_forca_atual[j][1]*matriz_tempo[j][1] - g*matriz_tempo[j][1]*matriz_tempo[j][1]/2;


                }

                //movimento em x
                matriz_translado[j][0] = matriz_pos_inicial[j][0] + matriz_forca_atual[j][0]*matriz_tempo[j][0];


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
    }