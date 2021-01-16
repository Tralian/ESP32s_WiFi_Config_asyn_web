
#include "main.h"

#include "bsp.h"
#include "mywifi.h"

System_infor_t sw;
/**
  * @brief	/led  handler
  */
void Web_led_handler(String inputMessage1,String inputMessage2) 
{
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    if(inputMessage1!="None"&&inputMessage2!="None")
    {
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }

}
/**
  * @brief	/wificonf  handler
  */
void  Web_wificonf_handler(String mySSID,String myPassword) 
{
    BSP_EEprom_WIFIconfig(mySSID,myPassword);
}
void setup()
{
  // Serial port for debugging purposes
  BSP_UART_Init(115200);
  BSP_EEprom_Init();
  
  BSP_EEprom_Read(EEPROM_SSID,&sw);

  BSP_EEprom_Read(EEPROM_PASSWORD,&sw);
  
  Serial.println("SSID leangth "+String(sw.SSID_len));
  Serial.println("SSID : "+sw.SSID);
  Serial.println("Password leangth "+String(sw.Password_len));
  Serial.println("Password : "+sw.Password);

  BSP_LED_Init();
  
  myWIFI_scan_ssid();
  
  myWIFI_connect();

  myWIFI_Webserver();
}

void loop() 
{

}

/**
	* @brief	modify analog watchdog setting in run time, wrapper function
	* @param	lower bound for adc monitor in mV
	* @param	upper bound for adc monitor in mV
	* @retval	VDD in mV.
  * @note		@ref BSP_MT_ConfigADCBattMonitor must be called first.
	* 				This routine does NOT initialize ADC, ONLY modify the threshold registers.
	*/