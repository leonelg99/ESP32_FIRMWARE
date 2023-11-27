#include "LibreriaWIFI.h"
#include "LibreriaFreeRTOS.h"
#include "messagesUtilities.h"

#define ledPin 4

void setup() {
  initSemaphores();
  uartSetup();
  startWiFi();
  serverSetup();
  cameraSetup();
  Serial.println("Hola");
  createTasks();
  Serial.println("Pues llegue!");
  
}

void loop() {
 
}
