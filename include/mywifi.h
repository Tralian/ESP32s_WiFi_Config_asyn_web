#ifndef MYWIFI_h
#define MYWIFI_h

#include <main.h>

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

void myWIFI_scan_ssid(void);
bool myWIFI_connect(System_infor_t *me);
void myWIFI_Webserver(void);
void myWIFI_AP_Webserver(void);

#endif
