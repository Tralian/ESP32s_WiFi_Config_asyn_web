
#include "mywifi.h"
#include "bsp.h"
/**
	* @brief	modify analog watchdog setting in run time, wrapper function
	* @param	lower bound for adc monitor in mV
	* @param	upper bound for adc monitor in mV
	* @retval	VDD in mV.
  * @note		@ref BSP_MT_ConfigADCBattMonitor must be called first.
	* 				This routine does NOT initialize ADC, ONLY modify the threshold registers.
	*/
void setup()
{
  // Serial port for debugging purposes
  BSP_UART_Init(115200);

  BSP_LED_Init();
  
  myWIFI_scan_ssid();
  
  myWIFI_connect();

  myWIFI_Webserver();
}

void loop() 
{

}

