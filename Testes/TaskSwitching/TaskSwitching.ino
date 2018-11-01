/*
Teste para determinar a velocidade de alterância entre tarefas.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>

#define STACK_DEPTH 100
#define LOOP_COUNT 10000

void vMainTask();
void vInterweavingTask();

void setup() {
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  xTaskCreate(vMainTask, NULL, 500, NULL, 4, NULL);
  xTaskCreate(vInterweavingTask, NULL, STACK_DEPTH, NULL, 4, NULL);  
  vTaskStartScheduler(); /* Inicia o escalonador */  

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vMainTask(){
  volatile uint32_t count = LOOP_COUNT;
  uint32_t t1, t2;
  float tl, ty, TS;
  
  t1 = micros();
  do{
  }while(count--);
  t2 = micros();
  tl = ((float)t2-t1)/(float)LOOP_COUNT; 
  count = LOOP_COUNT;
  t1 = micros();
  do{
    taskYIELD();
  }while(count--);
  t2 = micros();
  ty = (t2-t1)/ (float)LOOP_COUNT/2.0;
  TS = ty-tl;
  Serial.println(TS);
  vTaskDelete(NULL);
}

void vInterweavingTask(){
  do{
    taskYIELD();
  }while(1);
}

void loop() {}
