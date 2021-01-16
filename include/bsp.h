#ifndef BSP_h
#define BSP_h
#include <main.h>


void BSP_LED_Init(void);
void BSP_UART_Init(uint32_t bardrate);
void BSP_EEprom_Init(void);
void BSP_EEprom_Read(EEprom_t infro,System_infor_t * me);
void BSP_EEprom_WIFIconfig(String mySSID,String myPassword);

#endif