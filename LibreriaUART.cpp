#include "LibreriaUART.h"

#define ledPin 2
String msg;

void uartSetup(){
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
}

void uartStart(){
  while(1){
    if(Serial.available()){
      digitalWrite(ledPin,HIGH);
      msg=Serial.readStringUntil('\n');
      saveInBufferWIFI(msg);
      digitalWrite(ledPin,LOW);
    }
    if(getCommand(&msg)){
      Serial.println(msg);
    }
    vTaskDelay(2);
  }
}