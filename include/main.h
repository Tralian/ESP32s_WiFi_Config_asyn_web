#ifndef MAIN_h
#define MAIN_h
#include <Arduino.h>


#define EEPROM_SIZE 512
#define EE_ADD_SSID_LEN 0
#define EE_ADD_PW_LEN 1
#define EE_ADD_SSID_START 10
#define EE_ADD_PW_START 60

typedef enum
{
	EEPROM_SSID = 0,			
	EEPROM_PASSWORD,
}EEprom_t;



typedef struct
{

	String SSID;
    uint8_t SSID_len;
	String Password;
    uint8_t Password_len;
    

}System_infor_t;




#endif