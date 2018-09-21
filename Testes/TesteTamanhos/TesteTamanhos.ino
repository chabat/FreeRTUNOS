#include <FreeRTOS_AVR.h>
#include <basic_io_avr.h>

void setup() {
  uint8_t i; // 1 byte.
  int tam;
  Serial.begin(9600);
  // wait for Leonardo
  while(!Serial) {}

  Serial.print("Tamanho do(a) ListItem_t:");
  Serial.println(sizeof(ListItem_t));
  Serial.print("Tamanho do(a) List_t :");
  Serial.println(sizeof(List_t));
  Serial.print("Tamanho do(a) MiniListItem_t :");
  Serial.println(sizeof(MiniListItem_t));
  Serial.print("Tamanho do(a) TimerCallbackFunction_t :");
  Serial.println(sizeof(TimerCallbackFunction_t));

  
  // start FreeRTOS
  tam = vTaskStartScheduler();
  Serial.print("Tamanho do(a) Timer_t:");
  Serial.println(tam);
  
  // should never return
  Serial.println(F("Die"));
  while(1);
}

void loop() {
  // put your main code here, to run repeatedly:

}
