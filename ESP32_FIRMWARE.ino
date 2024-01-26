#include "LibreriaWIFI.h"
#include "LibreriaFreeRTOS.h"
#include "messagesUtilities.h"

#define ledPin 4

void setup() {
  initBuffers();
  initSemaphores();
  uartSetup();
  startWiFi();
  serverSetup();
  cameraSetup();
  createTasks();
}

void loop() {
 
}
