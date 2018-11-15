/*
Teste para determinar a velocidade de Comunicação entre tarefas utilizando filas.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

#define STACK_DEPTH 100
#define LOOP_COUNT 10000
#define MESSAGE_LENGTH 100
#define QUEUE_LENGTH 1

void vClient();
void vServer();


char message[MESSAGE_LENGTH];
QueueHandle_t fila1, fila2;

void setup() {
  for(int i = 0; i < MESSAGE_LENGTH; i++) message[i] = 'A';
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  fila1 = xQueueCreate(QUEUE_LENGTH, MESSAGE_LENGTH);
  fila2 = xQueueCreate(QUEUE_LENGTH, MESSAGE_LENGTH);
  xTaskCreate(vClient, NULL, STACK_DEPTH, NULL, 4, NULL);
  xTaskCreate(vServer, NULL, STACK_DEPTH, NULL, 4, NULL);  
  vTaskStartScheduler(); /* Inicia o escalonador */  

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vClient(){
  volatile uint32_t count;
  uint32_t t1, t2;
  float TM;

  count = LOOP_COUNT;
  t1 = micros();
  do{
    xQueueSend(fila1, message, portMAX_DELAY);
    xQueueReceive(fila2, message, portMAX_DELAY);
  }while(count--);
  t2 = micros();
  TM = (t2-t1)/(double)LOOP_COUNT/2.;
  Serial.println(TM);
  vTaskDelete(NULL);
}

void vServer(){
  do{
    xQueueReceive(fila1, message, portMAX_DELAY);
    xQueueSend(fila2, message, portMAX_DELAY);
  }while(1);
}

void loop() {}
