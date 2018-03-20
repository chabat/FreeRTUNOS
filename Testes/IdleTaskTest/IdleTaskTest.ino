/*
Programa para testar a quantidade de processamento
que a tarefa idle pode realizar sem comprometer a
estabilidade da aplicação.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>
#define mainDELAY_LOOP_COUNT 112345 /* Usado como contador para o loop, para criar um delay */
#define mainTASK_QUANTITY 4 /*Quantidade de tarefas a serem criadas*/
#define mainSTACK_DEPTH 200 /*Tamanho da pilha de cada tarefa em bytes*/

typedef struct{
  int iId;
  TaskHandle_t handle;
}param_t;

void vTarefa(void *pvParametros);

void setup() {
  int i;
  param_t parameters[mainTASK_QUANTITY + 1];
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */
  
  for(i = 1; i <= mainTASK_QUANTITY; i++){
    parameters[i].iId = i;
    xTaskCreate( vTarefa, "Tarefa", mainSTACK_DEPTH, (void*) &parameters[i], 1, &parameters[i].handle);
  }

  vTaskStartScheduler(); /* Inicia o escalonador */  

  Serial.print("E morreu\n");
  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vTarefa(void *pvParametro){
  volatile unsigned long ul; /*Variável para o contador*/
  param_t *parametro = (param_t*)pvParametro;
  int iId = parametro->iId, first = 1;
  TaskHandle_t handle = parametro->handle;

  for( ;; ){ /* Tarefas são criadas em loops infinitos */
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++); /* Implementação bruta de um atraso */  
    
    if(first){
      Serial.print("Tarefa "); /* Imprime a string */
      Serial.print(parametro->iId);
      Serial.print(" Executando\n");
    } 
    first = 0;   
    vTaskDelete(handle);
  }
}

void loop() {}
