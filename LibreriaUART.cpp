#include "LibreriaUART.h"

String msg;

void uartSetup(){
  Serial.begin(115200);
}

void uartStart(){
  while(1){
    if(Serial.available()){
      msg=Serial.readStringUntil('\n');
      saveInBufferUART(msg);
    }
    if(getCommand(*msg)){
      Serial.println(msg);
    }
    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}