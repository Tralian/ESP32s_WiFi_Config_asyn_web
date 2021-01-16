#include <Arduino.h>


void BSP_LED_Init(void)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW); 
}
void BSP_UART_Init(uint32_t bardrate)
{
  Serial.begin(bardrate);
}