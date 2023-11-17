#include "LibreriaUART.h"

#define ledPin 4
String msg;

void uartSetup(){
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
}

void uartStart(){
  while(1){
    if(Serial.available()){
      msg=Serial.readStringUntil('\n');
      saveInBufferWIFI(msg);
    }
    if(getCommand(&msg)){
      digitalWrite(ledPin,HIGH);
      Serial.println(msg);
      digitalWrite(ledPin,LOW);
    }
    msg="";
    vTaskDelay(10);
  }
}