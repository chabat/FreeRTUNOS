#include <Arduino_FreeRTOS.h>

/*
 * Este teste irá criar várias tarefas e vários mutexes, com o aumento de tarefas, o numero de mutexes
 * disponível será menor.
 */

/** current begining of heap */
extern char *__brkval;

#if defined(CORE_TEENSY) || ARDUINO == 104
extern char __bss_end;
/** \return free heap size */
size_t freeHeap() {
  return (char*)RAMEND - (__brkval ? __brkval : &__bss_end)+ 1;
}
#else  // CORE_TEENSY
/** \return free heap size */
size_t freeHeap() {
  return (char*)RAMEND - (__brkval ? __brkval : __malloc_heap_start) + 1;
}
#endif  // CORE_TEENSY

typedef struct{
  uint8_t tarefa, mutex;
} xQueueItem_t;

const uint8_t MAXT = 3; // 1 byte.
const uint8_t MAXV = 3; // 1 byte.
SemaphoreHandle_t xMutex[MAXV]; // 33 * MAXV  bytes quando inicializados, 2 na declaração.
QueueHandle_t xQueue; // 33 bytes da queue criada quanda inicializada, 2 na declaração.

//------------------------------------------------------------------------------

static void vGetSem(void* pvParameters){
  uint8_t TimerId = (int)pvParameters;
  uint8_t i;
  xQueueItem_t item;
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

//------------------------------------------------------------------------------

static void vPrintQueue(void* pvParameters){
  BaseType_t xStatus;
  xQueueItem_t item;
  for(;;){
    xStatus = xQueueReceive(xQueue, &item, portMAX_DELAY);
    if(xStatus == pdPASS ){
      Serial.print("Task ");
      Serial.print(item.tarefa);
      Serial.print(" got mutex ");
      Serial.println(item.mutex);
      Serial.println(freeHeap());
    }
  }
}

//------------------------------------------------------------------------------

void setup() {
  uint8_t i;
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}
  
  xQueue = xQueueCreate(1, sizeof(xQueueItem_t));

  Serial.println(freeHeap());
  for(i = 0; i < MAXT; i++){
    xTaskCreate(  vGetSem, // Nome da tarefa (funcao que a chama);
                  "TarefaTeste",      // Nome dado a tarefa;
                  100,                 // Quanto de pilha usar;
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

  Serial.println(freeHeap());
  for(i = 0; i < MAXV; i++){
    xMutex[i] = xSemaphoreCreateMutex();
    Serial.print(F("Memoria livre: "));
    Serial.println(freeHeap());
  }


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
