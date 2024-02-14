#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
//#include "LibreriaFreeRTOS.h"
#include "messagesUtilities.h"
#include "LibreriaCamera.h"

void startWiFi(void);
void serverSetup(void);
void serverExecute(void);
WiFiClient getClient(void);
