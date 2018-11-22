/*
Teste para testar o funcionamento de notificações de tarefas.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>

#define STACK_DEPTH 300
#define NUM 12345
#define DELAY 200

void vClient();
void vServer();

TaskHandle_t tClient, tServer;

void setup() {  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */
  
  xTaskCreate(vClient, "Cliente", STACK_DEPTH, NULL, 4, &tClient);  
  xTaskCreate(vServer, "Server", STACK_DEPTH, NULL, 4, &tServer);

    
  vTaskStartScheduler(); /* Inicia o escalonador */  

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vClient(){  

  while(1){
    if(xTaskNotify(tServer, NUM ,eSetValueWithOverwrite) == pdTRUE) Serial.println("Mensagem Enviada");
    else Serial.println("Falha no Envio");
    vTaskDelay(1000);
  }
  
  vTaskDelete(NULL);
}

void vServer(){
  uint32_t val;

  while(1){
    if(xTaskNotifyWait(1, 2, &val, portMAX_DELAY) == pdTRUE) {Serial.print("Mensagem Recebida: "); Serial.println(val);}
    else {Serial.print("Falha no recebimento: "); Serial.println(val);}
  }
  
  vTaskDelete(NULL);
}

void loop() {}
