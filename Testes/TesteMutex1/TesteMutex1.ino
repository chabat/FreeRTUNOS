// Contador para ver quantos timers ele suporta;
#include <FreeRTOS_AVR.h>

const uint8_t LED_PIN = 13;
const int MAXT = 1;
const int MAXV = 14;
int count[MAXV];
SemaphoreHandle_t xMutex[MAXV], xMMutex;
//------------------------------------------------------------------------------

static void vPrintTime(void* pvParameters){
  int TimerId = (int)pvParameters;
  unsigned long ulTimeStart = 0, ulTimeEnd = 0, ulTimeDif;
  for(;;){
    count[TimerId]++;
    xSemaphoreTake(xMMutex, portMAX_DELAY);
    ulTimeStart = micros();
    for(int i = 0; i < MAXV; i++){
      xSemaphoreTake(xMutex[i], portMAX_DELAY);
    }
    ulTimeEnd = micros();
    for(int i = 0; i < MAXV; i++){
      xSemaphoreGive(xMutex[i]); 
    }
    ulTimeDif = ulTimeEnd - ulTimeStart;
    Serial.print("Entrei na regiao critica no tempo: ");
    Serial.println(ulTimeDif);
    xSemaphoreGive(xMMutex);
  }
}

//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}
  xMMutex = xSemaphoreCreateMutex();
  for(int i = 0; i < MAXV; i++){
    xMutex[i] = xSemaphoreCreateMutex();
  }
  for(int i = 0; i < MAXT; i++){
    count[i] = 0;
    xTaskCreate( vPrintTime, // Nome da tarefa (funcao que a chama;
                              "TarefaTeste",      // Nome dado a tarefa;
                              200,                 // Quanto de pilha usar;
                              (void*)i,
                              tskIDLE_PRIORITY+2,                      // Prioridade;
                              NULL
                              );
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
