#include <Arduino_FreeRTOS.h>

void vCriadora(void);
void vCriada(void);

void setup() {
  Serial.begin(9600); /* Define a taxa de bits por segundo para transmissão de dados */

  xTaskCreate( vCriadora, NULL, 500, NULL, 4, NULL);
  //xTaskCreate( vCriadora, NULL, 500, NULL, 4, NULL);

  vTaskStartScheduler(); /* Inicia o escalonador */  

  for( ;; ); /* Se tudo deu certo, este trecho nunca será executado */
}

void vCriadora(void){
  Serial.println(F("Vou criar"));
  //xTaskCreate( vCriada, NULL, 100, NULL, 4, NULL);
  vTaskDelete(NULL);
}

void vCriada(void){
  Serial.println(F("Fui criada"));  
  
  vTaskDelete(NULL);
}

void loop() {}
