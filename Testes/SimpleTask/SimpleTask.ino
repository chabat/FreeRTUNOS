/*
Programa de teste base do FreeRTOS no Arduino UNO,
Apenas uma tarefa que imprime uma string com um contador
e depois espera pelo fim de um tempo de delay.

Authors: Felipe Chabatura Neto
         Leonardo Tironi Fassini
*/
#include <Arduino_FreeRTOS.h>

/* Usado como contador para o loop, para criar um delay */ 
#define mainDELAY_LOOP_COUNT   ( 0xfffff )

void vTarefa(void *pvParametros);

void setup() {
  /* Define a taxa de bits por segundo para transmissão de dados */ 
  Serial.begin(9600);

  /* Cria a tarefa */
  xTaskCreate( vTarefa, "Tarefa", 200, NULL, 1, NULL );

  /* Inicia o escalonador */
  vTaskStartScheduler();  
    
  /* Se tudo deu certo, este trecho nunca será executado */
  for( ;; );
}

void vTarefa(void *pvParametros){
  int count = 0;
  const char *pcString = "Tarefa Executando! ";
  volatile unsigned long ul;

  /* Tarefas são criadas em loops infinitos */
  for( ;; ){
    /* Imprime a string */
    Serial.print(pcString);
    Serial.println(count++); 
    //vPrintString(pcString);

    /* Implementação bruta de um atraso */ 
    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++);
  }
}

void loop() {}
