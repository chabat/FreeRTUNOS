/*
Teste para determinar a velocidade de alterância entre tarefas.

Authors: Felipe Chabatura Neto
*/

#include <Arduino_FreeRTOS.h>

#define STACK_DEPTH 100
#define LOOP_COUNT 1000000

void vMainTask();
void vInterweavingTask();

void setup() {
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  xTaskCreate(vMainTask, NULL, 800, NULL, 5, NULL);
  xTaskCreate(vInterweavingTask, NULL, 100, NULL, 4, NULL);  
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
  Serial.print(F("Tempo do laço sozinho: ")); Serial.print((t2-t1)/1000.); Serial.println(F(" ms"));
  Serial.print(F("Tempo por iteração: ")); Serial.print(tl); Serial.println(F(" μs"));
  count = LOOP_COUNT;
  t1 = micros();
  do{
    taskYIELD();
  }while(count--);
  t2 = micros();
  ty = (t2-t1)/ (float)LOOP_COUNT/2.0;
  TS = ty-tl;
  Serial.print(F("Tempo do laço com troca de contexto: ")); Serial.print((t2-t1)/1000.); Serial.println(F(" ms"));
  Serial.print(F("Tempo da troca de contexto: ")); Serial.print(TS); Serial.println(F(" μs"));
  vTaskDelete(NULL);
}

void vInterweavingTask(){
  do{
    taskYIELD();
  }while(1);
}

void loop() {}
