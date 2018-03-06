#include <Arduino_FreeRTOS.h>

#define mainDELAY_LOOP_COUNT   ( 0xfffff ) /* Usado como contador para o loop, para criar um delay */

void vTarefa(void *pvParametros);

void setup() {
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  /* Cria a tarefa */
  xTaskCreate( vTarefa, /*Ponteiro para a função da tarefa*/
              "Tarefa", /*Nome da tarefa (apenas para debug)*/
              200, /*Tamanho da pilha*/
              NULL,/*Parametro passado a tarefa*/
              1, /*Prioridade*/
              NULL /*O handle da tarefa*/ );

  vTaskStartScheduler(); /* Inicia o escalonador */

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vTarefa(void *pvParametros){
  const char *pcString = "Tarefa Executando!\n"; /*String que a tarefa imprime*/
  volatile unsigned long ul; /*Variável para o contador*/

  for( ;; ){ /* Tarefas são criadas em loops infinitos */
    Serial.print(pcString); /* Imprime a string */

    for(ul = 0; ul < mainDELAY_LOOP_COUNT; ul++); /* Implementação bruta de um atraso */
  }
}

void loop() {}
