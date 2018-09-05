/*
 * Esse teste irá pegar o mutex e adicionar na fila que conseguiu pegar o mutex i;
 */

#include <FreeRTOS_AVR.h>

typedef struct{
  uint8_t tarefa, mutex;
} xQueueItem_t;

const uint8_t LED_PIN = 13;
const uint8_t MAXT = 2;
const uint8_t MAXV = 6;
SemaphoreHandle_t xMutex[MAXV], xMMutex;
QueueHandle_t xQueue;
const TickType_t BlockTime = pdMS_TO_TICKS(200);
//------------------------------------------------------------------------------
  
static void vGetTime(void* pvParameters){
  uint8_t TimerId = (int)pvParameters;
  uint8_t i;
  xQueueItem_t item;
  for(;;){
    //ulTimeStart = micros();
    for(i = 0; i < MAXV; i++){
      xSemaphoreTake(xMutex[i], portMAX_DELAY);
      item.tarefa = TimerId;
      item.mutex = i;
      xQueueSendToBack(xQueue, &item, portMAX_DELAY);
      //vTaskDelay(BlockTime);
    }
    for(i = 0; i < MAXV; i++){
      xSemaphoreGive(xMutex[i]); 
    }
  }
}

static void vPrintTime(void* pvParameters){
  uint8_t TimerId = (int)pvParameters;
  uint8_t i;
  xQueueItem_t item;
  for(;;){
    for(i = 0; i < MAXV; i++){
      xSemaphoreTake(xMutex[i], portMAX_DELAY);
      item.tarefa = TimerId;
      item.mutex = i;
      xQueueSendToBack(xQueue, &item, portMAX_DELAY);      
      //vTaskDelay(BlockTime);
    }
    for(i = 0; i < MAXV; i++){
      xSemaphoreGive(xMutex[i]); 
    }
  }
}

static void vPrintQueue(void* pvParameters){
  BaseType_t xStatus;
  xQueueItem_t item;
  for(;;){

    if(uxQueueMessagesWaiting(xQueue) != 0){
      Serial.println("Queue should have been empty");
    }
    
    xStatus = xQueueReceive(xQueue, &item, portMAX_DELAY);

    if(xStatus == pdPASS ){
      Serial.print("Task ");
      Serial.print(item.tarefa);
      Serial.print(" got mutex ");
      Serial.println(item.mutex);
    }
    else{
      Serial.println("Could not get data from the queue");
    }
  }
}

//------------------------------------------------------------------------------

void setup() {
  uint8_t i;
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}
  xMMutex = xSemaphoreCreateMutex();
  for(i = 0; i < MAXV; i++){
    xMutex[i] = xSemaphoreCreateMutex();
  }
  
  xQueue = xQueueCreate(2, 10);

  xTaskCreate(  vGetTime, // Nome da tarefa (funcao que a chama);
                  "TarefaTeste",      // Nome dado a tarefa;
                  200,                 // Quanto de pilha usar;
                  (void*)0,
                  tskIDLE_PRIORITY+2,                      // Prioridade;
                  NULL );

                  
  xTaskCreate(  vPrintTime,
                "TarefaQuePrinta",
                200,
                (void*)1,
                tskIDLE_PRIORITY+2,
                NULL );

  xTaskCreate ( vPrintQueue,
                "TarefaPrintaQueue",
                200,
                NULL,
                tskIDLE_PRIORITY+3,
                NULL );
  Serial.println("Criou");

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
