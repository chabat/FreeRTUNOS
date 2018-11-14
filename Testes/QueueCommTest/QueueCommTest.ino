/*
Teste para determinar a velocidade de Comunicação entre tarefas utilizando filas.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>
#include <queue.h>

#define STACK_DEPTH 300
#define LOOP_COUNT 10000
#define MESSAGE_SIZE 1
#define QUEUE_LENGTH 5
#define TICKS_TO_WAIT 500

void vClient();
void vServer();


char message[MESSAGE_SIZE];

void setup() {
  QueueHandle_t fila;
  for(int i = 0; i < MESSAGE_SIZE; i++) message[i] = 'A';
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  fila = xQueueCreate(QUEUE_LENGTH, MESSAGE_SIZE);
  xTaskCreate(vClient, NULL, STACK_DEPTH, (void*) fila, 4, NULL);
  xTaskCreate(vServer, NULL, STACK_DEPTH, (void*) fila, 4, NULL);  
  vTaskStartScheduler(); /* Inicia o escalonador */  

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vClient(void* param){
  volatile uint32_t count = LOOP_COUNT;
  uint32_t t1, t2;
  float TM;
  QueueHandle_t fila = param;  

  Serial.println(message);
  t1 = micros();
  do{
    xQueueSend(fila, message, TICKS_TO_WAIT);
    Serial.println("Enviei");
  }while(count--);
  t2 = micros();
  TM = (t2-t1)/count/2.;
  Serial.println(TM);
  vTaskDelete(NULL);
}

void vServer(void* param){
  QueueHandle_t fila = param;  
  do{
    //Le da fila
    //Escreve na fila
  }while(1);
}

void loop() {}
