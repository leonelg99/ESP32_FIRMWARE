#include "LibreriaWIFI.h"
#include "LibreriaFreeRTOS.h"
#include "messagesUtilities.h"

void setup() {
  initSemaphores();
  uartSetup();
  startWiFi();
  serverSetup();
  createTasks();
}
void loop() {

}
