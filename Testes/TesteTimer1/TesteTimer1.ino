#include <Arduino_FreeRTOS.h>
#include <timers.h>
/* 
 * Programa para testar o uso de timers em uma aplicaçao  
 */

const uint8_t MAXT = 5; // 1 byte.
const uint8_t MAXV = 1; //  byte.
TimerHandle_t xTimer[MAXV]; // 65 * MAXV  bytes.
//QueueHandle_t xQueue; // 2 bytes + 63 bytes da queue criada.
const TickType_t BlockTime = pdMS_TO_TICKS(200); // 2 bytes.
//------------------------------------------------------------------------------

static void vGetTimer(void* pvParameters){
  int TimerId = (int)pvParameters;
  BaseType_t ret;
  int i;
  for(;;){
    for(i = 0; i < MAXV; i++){
      if(xTimerStart(xTimer[i], BlockTime) == pdFALSE)
        Serial.print("Erro ao iniciar timer\n");
    }
    vTaskDelay(BlockTime*10);
  }
}

//------------------------------------------------------------------------------

static void vPrintTimer(TimerHandle_t xTimer){
  uint8_t uTimerId;
  uTimerId = (uint8_t) pvTimerGetTimerID(xTimer);
  Serial.print(F("Sou o timer: "));
  Serial.println(uTimerId);
}

//------------------------------------------------------------------------------

void setup() {
  uint8_t i; // 1 byte.
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}
  
  for(i = 0; i < MAXV; i++){
    xTimer[i] = xTimerCreate( "Timer",
                              BlockTime,
                              pdFALSE,
                              (void*)i,
                              vPrintTimer
                            );
  }
  
  for(i = 0; i < MAXT; i++){
    xTaskCreate(  vGetTimer, // Nome da tarefa (funcao que a chama);
                  "TarefaTeste",      // Nome dado a tarefa;
                  200,                 // Quanto de pilha usar;
                  (void*)i,
                  tskIDLE_PRIORITY+1,                      // Prioridade;
                  NULL ); 
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
