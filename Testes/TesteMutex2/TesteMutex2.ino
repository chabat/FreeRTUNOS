// Contador para ver quantos timers ele suporta;
// Cria duas tarefas que irão ser deletadas após serem criadas.
// Ficam fazendo isso sempre.

#include <FreeRTOS_AVR.h>

const uint8_t LED_PIN = 13;
const uint8_t MAXT = 2;
const uint8_t MAXV = 2;
SemaphoreHandle_t xMutex[MAXV], xMMutex;
unsigned long ulTimeStart = 0, ulTimeEnd = 0, ulTimeDif;
unsigned long t1, t2;  
//------------------------------------------------------------------------------
  
static void vGetTime(void* pvParameters){
  uint8_t TimerId = (int)pvParameters;
  uint8_t i;
  for(;;){
    xSemaphoreTake(xMMutex, portMAX_DELAY);
    ulTimeStart = micros();
    for(i = 0; i < MAXV; i++){
      xSemaphoreTake(xMutex[i], portMAX_DELAY);
    }
    for(i = 0; i < MAXV; i++){
      xSemaphoreGive(xMutex[i]); 
    }
    xSemaphoreGive(xMMutex);
  }
}

static void vPrintTime(void* pvParameters){
  uint8_t TimerId = (int)pvParameters;
  uint8_t i;
  for(;;){
    xSemaphoreTake(xMMutex, portMAX_DELAY);
    for(i = 0; i < MAXV; i++){
      xSemaphoreTake(xMutex[i], portMAX_DELAY);
    }
    for(i = 0; i < MAXV; i++){
      xSemaphoreGive(xMutex[i]); 
    }
    ulTimeEnd = micros();
    //Serial.print(t1);
    //Serial.print(F(" "));
    //Serial.println(t2);
    ulTimeDif = ulTimeEnd - ulTimeStart;
    Serial.print("Entrei na regiao critica no tempo: ");
    Serial.println(ulTimeDif);
    xSemaphoreGive(xMMutex);
    vTaskDelay(300);
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
  /*
  for(i = 0; i < MAXT; i++){
    xTaskCreate( vPrintTime, // Nome da tarefa (funcao que a chama;
                              "TarefaTeste",      // Nome dado a tarefa;
                              200,                 // Quanto de pilha usar;
                              (void*)i,
                              tskIDLE_PRIORITY+2,                      // Prioridade;
                              NULL
                              );
  }*/

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
