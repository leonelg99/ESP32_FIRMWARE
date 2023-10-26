#include "LibreriaUART.h"

String msg;

void uartSetup(){
  Serial.begin(115200);
}

void uartStart(){
  while(1){
    if(Serial.available()){
      msg=Serial.readStringUntil('\n');
      saveInBufferWIFI(msg);
    }
    if(getCommand(&msg)){
      Serial.println(msg);
    }
    vTaskDelay(2);
  }
}