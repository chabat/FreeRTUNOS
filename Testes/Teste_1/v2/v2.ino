#include <Arduino_FreeRTOS.h>
 
#define mainDELAY_LOOP_COUNT   ( 0xfffff ) /* Usado como contador para o loop, para criar um delay */
#define mainTASK_QUANTITY 4

void vTarefa(void *pvParametros);

void setup() {
  int iId;
     
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  Serial.println(iFreeHeapSpace);
  
  for(iId = 1; iId <= mainTASK_QUANTITY; iId++){
    xTaskCreate( vTarefa, "Tarefa", 200, (void*) iId, 1, NULL);
  }
  
  Serial.println(iFreeHeapSpace);
  
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
