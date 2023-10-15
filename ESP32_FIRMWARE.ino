#include "LibreriaWIFI.h"
#include "LibreriaFreeRTOS.h"
#include "messagesUtilities.h"

void setup() {
  initSemaphores();
  startWiFi();
  createTasks();
}

void loop() {
  // put your main code here, to run repeatedly:

}
