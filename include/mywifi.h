#ifndef MYWIFI_h
#define MYWIFI_h


#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

void myWIFI_scan_ssid(void);
IPAddress myWIFI_connect(void);
void myWIFI_Webserver(void);

#endif
