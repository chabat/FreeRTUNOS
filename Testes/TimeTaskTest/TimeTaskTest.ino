/*
Programa para testar a quantidade de processamento
que a tarefa idle pode realizar sem comprometer a
estabilidade da aplicação.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#define mainDELAY_LOOP_COUNT 112345 /* Usado como contador para o loop, para criar um delay */
#define mainTASK_QUANTITY 2 /*Quantidade de tarefas a serem criadas*/
#define mainSTACK_DEPTH 300 /*Tamanho da pilha de cada tarefa em bytes*/
//#define mainUSE_FREERTOS_TICK_COUNT /*Se usa ou nao as funcoes do FreeRTOS para calcular o tempo*/

typedef struct{
  int iId;
  TaskHandle_t handle;
  SemaphoreHandle_t xPrintMutex;
}param_t;

void vTarefa(void *pvParametros);

void setup() {
  int i;
  param_t parameters[mainTASK_QUANTITY + 1];
  SemaphoreHandle_t xPrintMutex = xSemaphoreCreateMutex();
  
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  Serial.println(pdMS_TO_TICKS(1000));
  
  for(i = 1; i <= mainTASK_QUANTITY; i++){
    parameters[i].iId = i;
    parameters[i].xPrintMutex = xPrintMutex;
    xTaskCreate( vTarefa, "Tarefa", mainSTACK_DEPTH, (void*) &parameters[i], 1, &parameters[i].handle);
  }

  vTaskStartScheduler(); /* Inicia o escalonador */  

  Serial.print("E morreu\n");
  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vTarefa(void *pvParametro){
  TickType_t fimFre, inicioFre;
  unsigned long fimArd, inicioArd;
  volatile unsigned long ul; /*Variável para o contador*/
  param_t *parametro = (param_t*)pvParametro;
  int iId = parametro->iId, first = 1;
  TaskHandle_t handle = parametro->handle;
   
  for( ;; ){ /* Tarefas são criadas em loops infinitos */
    
    #ifdef mainUSE_FREERTOS_TICK_COUNT
      inicioFre = xTaskGetTickCount();
    #endif

    #ifndef mainUSE_FREERTOS_TICK_COUNT
      inicioArd = millis();
    #endif
    
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++); /* Implementação bruta de um atraso */  
    
    #ifdef mainUSE_FREERTOS_TICK_COUNT
      fimFre = xTaskGetTickCount();
    #endif

    #ifndef mainUSE_FREERTOS_TICK_COUNT
      fimArd = millis();
    #endif
    
    xSemaphoreTake(parametro->xPrintMutex, portMAX_DELAY);
    Serial.print("Tarefa "); Serial.print(iId);

    #ifdef mainUSE_FREERTOS_TICK_COUNT
      Serial.print(" I:"); Serial.print(inicioFre); Serial.print(" F:"); Serial.print(fimFre); 
    #endif
    
    Serial.print(" executou em ");

    #ifdef mainUSE_FREERTOS_TICK_COUNT
      Serial.print(1000*(fimFre-inicioFre)/pdMS_TO_TICKS(1000));
    #endif

    #ifndef mainUSE_FREERTOS_TICK_COUNT
      Serial.print(fimArd - inicioArd);
    #endif
    
    Serial.print("ms\n");
    xSemaphoreGive(parametro->xPrintMutex);
    
    //vTaskDelete(handle);
  }
}

void loop() {}
