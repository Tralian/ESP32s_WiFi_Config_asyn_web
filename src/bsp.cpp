#include"bsp.h"
#include <Arduino.h>
#include <EEPROM.h>
/**
  * @brief	LED GPIO Initial	
  */
void BSP_LED_Init(void)
{
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW); 
}
/**
  * @brief	UART Initial	
  */
void BSP_UART_Init(uint32_t bardrate)
{
  Serial.begin(bardrate);
}
/**
  * @brief	EEprom Initial	
  */
void BSP_EEprom_Init(void)
{
  EEPROM.begin(EEPROM_SIZE);
}
/**
  * @brief	Read data from EEprom and put into 	system infor
  */
void BSP_EEprom_Read(EEprom_t infro,System_infor_t *me)
{
  String eeprom_read="";
  switch(infro)
  {
    case EEPROM_SSID:
      me->SSID_len=EEPROM.read(EE_ADD_SSID_LEN);
      if(me->SSID_len!=255)// avoid empty EEprom
      {
        for (int i = 0; i < me->SSID_len; ++i)
        {
          eeprom_read+=EEPROM.read(EE_ADD_SSID_START+i);
          me->SSID=eeprom_read;
        }
      }
      break;
    case EEPROM_PASSWORD:
      me->Password_len=EEPROM.read(EE_ADD_PW_LEN);
      if(me->Password_len!=255)// avoid empty EEprom
      {
        for (int i = 0; i < me->Password_len; ++i)
        {
          eeprom_read+=EEPROM.read(EE_ADD_SSID_START+i);
          me->Password=eeprom_read;
        }
      }
      break;
  }
}
/**
  * @brief	Storge SSID to EEprom
  */
void BSP_EEprom_WIFIconfig(String mySSID,String myPassword)
{
  uint8_t SSID_len=mySSID.length();
  uint8_t PW_len=myPassword.length();

  char SSID_buf[SSID_len];
  char PW_buf[PW_len];
  
  mySSID.toCharArray(SSID_buf,SSID_len);
  myPassword.toCharArray(PW_buf,PW_len);

  String eeprom_read="";
  if(SSID_len!=0&&PW_len!=0)
  {
      EEPROM.write(EE_ADD_SSID_LEN,SSID_len);
      
      for(uint8_t i=0;i<SSID_len;i++)
      {
        EEPROM.write(EE_ADD_SSID_START+i,(int)SSID_buf[i]);
      }
      Serial.println("length of SSID ="+String(EEPROM.read(EE_ADD_SSID_LEN)));
      /* Check EEprom storge*/
      {
        eeprom_read="";
        for(uint8_t i=0;i<SSID_len;i++)
        {
        eeprom_read+=char(EEPROM.read(EE_ADD_SSID_START+i));
        }     
        Serial.println("SSID: "+eeprom_read);
      }

      EEPROM.write(EE_ADD_PW_LEN,PW_len);

      for(uint8_t i=0;i<PW_len;i++)
      {
        EEPROM.write(EE_ADD_PW_START+i,PW_buf[i]);
      }
      /* Check EEprom storge*/
      {
        Serial.println("length of Pass word ="+String(EEPROM.read(EE_ADD_PW_LEN)));

        eeprom_read="";
        for(uint8_t i=0;i<PW_len;i++)
        {
        eeprom_read+=char(EEPROM.read(EE_ADD_PW_START+i));
        }     
        Serial.println("Pass word: "+eeprom_read);
      }
           
  }

}