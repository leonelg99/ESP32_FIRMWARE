#include "LibreriaUART.h"

String msg;

//This function is used to set the baud rate for the UART and turn on a LED
void uartSetup(){
  Serial.begin(115200);
  
}

//This function is used for the communication through UART
//First, if there is data in the UART, it is read and saved in a buffer,
//If there is a message for the EDU-CIAA, it is send through UART to the MCU
void uartStart(){
  while(1){
    if(Serial.available()){
      msg=Serial.readStringUntil('\n');
      saveInBufferWIFI(msg);
    }
    if(getCommand(&msg)){
      Serial.println(msg);
    }
    msg="";
    vTaskDelay(10);
  }
}
