/*
Programa de teste base do FreeRTOS no Arduino UNO,
Apenas uma tarefa que imprime uma string com um contador
e depois espera pelo fim de um tempo de delay.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/
#include <Arduino_FreeRTOS.h>

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

/* Usado como contador para o loop, para criar um delay */ 
//#define mainDELAY_LOOP_COUNT   ( 0xfffff )

//void vTarefa(void *pvParametros);

void setup() {
  /* Define a taxa de bits por segundo para transmissão de dados */ 
  /* Cria a tarefa */
  //xTaskCreate( vTarefa, "Tarefa", 200, NULL, 1, NULL );

  /* Inicia o escalonador */
  vTaskStartScheduler();  
    
  /* Se tudo deu certo, este trecho nunca será executado */
  for( ;; );
}

/*
void vTarefa(void *pvParametros){
  int count = 0;
  const char *pcString = "Tarefa Executando! ";
  volatile unsigned long ul;

  // Tarefas são criadas em loops infinitos
  for( ;; ){
    Imprime a string
    Serial.print(sizeof(UBaseType_t));
    Serial.print(pcString);
    Serial.println(count++); 
    //vPrintString(pcString);

    Implementação bruta de um atraso
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++);
  }
}
*/
void loop() {}
