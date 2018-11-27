/*
Cria mainTASK_QUANTITY tarefas que executam alternadamente. 

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>
#define mainDELAY_LOOP_COUNT 112345 /* Usado como contador para o loop, para criar um delay */
#define mainTASK_QUANTITY 10 /*Quantidade de tarefas a serem criadas*/
#define mainSTACK_DEPTH 89 /*Tamanho da pilha de cada tarefa em bytes*/

void vTarefa(void);

void setup() {
  
  for(int i = 0; i < mainTASK_QUANTITY; i++)
    xTaskCreate( vTarefa, NULL, mainSTACK_DEPTH, NULL, 3, NULL);
  
  vTaskStartScheduler(); /* Inicia o escalonador */  

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vTarefa(void){
  volatile unsigned long ul; /*Variável para o contador*/

  for( ;; ){ /* Tarefas são criadas em loops infinitos */  
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++); /* Implementação bruta de um atraso */ 
  }
}

void loop() {}
