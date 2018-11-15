/*
Teste para determinar a velocidade de Comunicação entre tarefas utilizando Notificações de Tarefas.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>

#define STACK_DEPTH 100
#define LOOP_COUNT 10000
#define NUM 12345
#define DELAY 50
#define ULONG_MAX 0xffffffff

void vClient();
void vServer();

TaskHandle_t tClient, tServer;

void setup() {  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */
  
  tServer = xTaskCreate(vServer, NULL, STACK_DEPTH, NULL, 4, NULL);
  tClient = xTaskCreate(vClient, NULL, STACK_DEPTH, NULL, 4, NULL);  
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
    xTaskNotify(tServer, NUM ,eIncrement);
    //xTaskNotifyWait(NULL, NULL, NULL, DELAY);
    delay(50);
  }while(count--);
  t2 = micros();
  TM = (t2-t1)/(double)LOOP_COUNT/2.;
  Serial.println(TM);
  vTaskDelete(NULL);
}

void vServer(){
  uint32_t val;
  do{
    xTaskNotifyWait(ULONG_MAX, &val, NULL, DELAY);
    Serial.println(val);
    //xTaskNotify(tClient, NUM ,eSetValueWithOverwrite);
  }while(1);
  vTaskDelete(NULL);
}

void loop() {}
