#include "LibreriaWIFI.h"
#include "LibreriaFreeRTOS.h"
#include "messagesUtilities.h"

#define ledPin 4

void setup() {
  initSemaphores();
  uartSetup();
  startWiFi();
  serverSetup();
  createTasks();
 /* Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,HIGH);
  delay(1000);
  digitalWrite(ledPin,LOW);*/
}
String pepe;
void loop() {
 /* if(Serial.available() > 0){
      digitalWrite(ledPin,HIGH);
      delay(200);
      pepe=Serial.readStringUntil('\n');
      Serial.print(pepe);
      //saveInBufferWIFI(pepe);
      digitalWrite(ledPin,LOW);
      delay(500);
    }*/
}
