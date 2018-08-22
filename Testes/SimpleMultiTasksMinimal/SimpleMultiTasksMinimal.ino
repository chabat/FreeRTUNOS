/*
Cria mainTASK_QUANTITY tarefas que executam alternadamente,
imprimindo uma string. 

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>
#include <ArduinoTestSuite.h>
#define mainDELAY_LOOP_COUNT 112345 /* Usado como contador para o loop, para criar um delay */
#define mainSTACK_DEPTH 1089 /*Tamanho da pilha de cada tarefa em bytes*/

void vTarefa(void *pvParametros);

void setup() {
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */
  
  Serial.println(ATS_GetFreeMemory());
    
  xTaskCreate( vTarefa, "Tarefa", mainSTACK_DEPTH, (void*) 1, 1, NULL);

  Serial.println(ATS_GetFreeMemory());
    
  vTaskStartScheduler(); /* Inicia o escalonador */
  //Serial.print("A");  
  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vTarefa(void *pvParametro){
  volatile unsigned long ul; /*Variável para o contador*/
  int iId = (int)pvParametro;

  //for( ;; ){ /* Tarefas são criadas em loops infinitos */
    Serial.print("Tarefa "); /* Imprime a string */
    Serial.print(iId);
    Serial.print(" Executando\n");
    
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++); /* Implementação bruta de um atraso */ 
  //}
}

void loop() {}
