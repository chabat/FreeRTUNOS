#include <Arduino_FreeRTOS.h>
 
#define mainDELAY_LOOP_COUNT 112345 /* Usado como contador para o loop, para criar um delay */
#define mainTASK_QUANTITY 5 /*Quantidade de tarefas a serem criadas*/
#define mainSTACK_DEPTH 100 /*Tamanho da pilha de cada tarefa em bytes*/

void vTarefa(void *pvParametros);

void setup() {
  int iId;
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */
  
  for(iId = 1; iId <= mainTASK_QUANTITY; iId++){
    xTaskCreate( vTarefa, "Tarefa", mainSTACK_DEPTH, (void*) iId, 1, NULL);
  }

  vTaskStartScheduler(); /* Inicia o escalonador */  

  Serial.print("E morreu\n");
  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vTarefa(void *pvParametro){
  volatile unsigned long ul; /*Variável para o contador*/
  int iId = (int)pvParametro;

  for( ;; ){ /* Tarefas são criadas em loops infinitos */
    Serial.print("Tarefa "); /* Imprime a string */ 
    Serial.print(iId);
    Serial.print(" Executando\n");
    
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++); /* Implementação bruta de um atraso */ 
  }
}

void loop() {}
