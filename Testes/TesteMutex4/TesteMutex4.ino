/*
 * Este teste irá criar várias tarefas e vários mutexes, com o aumento de tarefas, o numero de mutexes
 * disponível será menor.
 */

#include <FreeRTOS_AVR.h>


typedef struct{
  uint8_t tarefa, mutex;
} xQueueItem_t;

const uint8_t MAXT = 4; // 1 byte.
const uint8_t MAXV = 1; // 1 byte.
SemaphoreHandle_t xMutex[MAXV]; // 65 * MAXV  bytes.
QueueHandle_t xQueue; // 2 bytes + 63 bytes da queue criada.
//const TickType_t BlockTime = pdMS_TO_TICKS(200); // 2 bytes.
//------------------------------------------------------------------------------

static void vGetSem(void* pvParameters){
  uint8_t TimerId = (int)pvParameters; // 1 byte. 
  uint8_t i; // 1byte.
  xQueueItem_t item; // 2 bytes.
  for(;;){
    for(i = 0; i < MAXV; i++){
      xSemaphoreTake(xMutex[i], portMAX_DELAY);
      item.tarefa = TimerId;
      item.mutex = i;
      xQueueSendToBack(xQueue, &item, portMAX_DELAY);      
    }
    for(i = 0; i < MAXV; i++){
      xSemaphoreGive(xMutex[i]); 
    }
  }
}

static void vPrintQueue(void* pvParameters){
  BaseType_t xStatus; // 1 byte.
  xQueueItem_t item; // 2 bytes.
  for(;;){
    xStatus = xQueueReceive(xQueue, &item, portMAX_DELAY);
    if(xStatus == pdPASS ){
      Serial.print("Task ");
      Serial.print(item.tarefa);
      Serial.print(" got mutex ");
      Serial.println(item.mutex);
    }
  }
}

//------------------------------------------------------------------------------

void setup() {
  uint8_t i; // 1 byte.
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}
  for(i = 0; i < MAXV; i++){
    xMutex[i] = xSemaphoreCreateMutex();
  }
  
  xQueue = xQueueCreate(1, sizeof(xQueueItem_t)); // 2 bytes. 

  for(i = 0; i < MAXT; i++){
    xTaskCreate(  vGetSem, // Nome da tarefa (funcao que a chama);
                  "TarefaTeste",      // Nome dado a tarefa;
                  200,                 // Quanto de pilha usar;
                  (void*)i,
                  tskIDLE_PRIORITY+2,                      // Prioridade;
                  NULL ); 
  }
  
  xTaskCreate ( vPrintQueue,
                "TarefaPrintaQueue",
                200,
                NULL,
                tskIDLE_PRIORITY+3,
                NULL );
  
  // start FreeRTOS
  vTaskStartScheduler();

  // should never return
  Serial.println(F("Die"));
  while(1);
}

//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
}
