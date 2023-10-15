#include "messagesUtilities.h"
#define MAX 15

//Buffers WIFI y UART
String bufferWIFI[MAX];
int indexWIFI_rx=0,indexWIFI_tx=0;
bool wifiRecived=0;

String bufferUART[MAX];
int indexUART_rx=0,indexUART_tx=0;
bool uartRecived=0;

SemaphoreHandle_t semUART,semWIFI;

SemaphoreHandle_t createSemaphore(){
  SemaphoreHandle_t semaphore = xSemaphoreCreateBinary();
  return semaphore;
}

void initSemaphores(){
  semUART = createSemaphore();
  if(semUART != NULL){
    xSemaphoreGive(semUART);
  }
  semWIFI = createSemaphore();
  if(semWIFI != NULL){
    xSemaphoreGive(semWIFI);
  }

}

void processMessage(String message_aux, String *resp, String *message){
  int ind1; // : location
  ind1 = message_aux.indexOf(':');  //finds location of first :
  *resp = message_aux.substring(0, ind1);   //captures first data String
  *message = message_aux.substring(ind1+1,'\n');
}

void processCamMessage(String message, String *cmd, String *value, String *value2){
  int ind1;
  int ind2;

  ind1 = message.indexOf(':');  //finds location of first :
  *cmd = message.substring(0, ind1);   //captures first data String
  ind2 = message.indexOf(':', ind1+1);
  *value = message.substring(ind1+1,ind2+1);
  *value2 = message.substring(ind2+1, '\n');
}
//Save in Buffer a msg receive in UART to be send later by wifi
bool saveInBufferWIFI(String msg){
  int error = 1;
  if(xSemaphoreTake(semWIFI,(TickType_t)5)==pdTRUE){
    if(indexWIFI<MAX){
      bufferWIFI[indexWIFI_rx]=msg;
      indexWIFI_rx++;
      wifiRecived=++;
      error=0;
    }
    xSemaphoreGive(semWIFI);
  }
  return error;
}

//Actualizar Indices de WIFI
void updateBufferWIFI(){
  if(indexWIFI_rx>=MAX)
    indefWIFI_rx=0;
  if(indexWIFI_tx>=MAX)
    indefWIFI_tx=0;
}

//Get command saved in bufferWIFI if there is one
bool getCommand(String *cmd){
  int exito=0;
  String msg;
  if(xSemaphoreTake(semWIFI,(TickType_t)5)==pdTRUE){
    if(wifiRecived){
      wifiReceived--;
      msg=bufferWIFI[indexWIFI_tx];
      indexWIFI_tx++;
      updateBufferWIFI();
      exito=1;
      *cmd=msg;
    }
    xSemaphoreGive(semWIFI);
  }
  return exito;
}


//Save in Buffer a msg receive by WIFI to be send later to EDU-CIAA by UART
bool saveInBufferUART(String msg){
  int error = 1;
  if(xSemaphoreTake(semUART,(TickType_t)5)==pdTRUE){
    if(indexUART<MAX){
      bufferWIFI[indexUART]=msg;
      indexUART++;
      uartRecived=++;
      error=0;
    }
    xSemaphoreGive(semUART);
  }
  return error;
}

//Actualizar Indices de WIFI
void updateBufferUART(){
  if(indexUART_rx>=MAX)
    indexUART_rx=0;
  if(indexUART_tx>=MAX)
    indexUART_tx=0;
}

//Get command saved in bufferWIFI if there is one
bool getMsg(String *msg){
  int exito=0;
  String str;
  if(xSemaphoreTake(semUART,(TickType_t)5)==pdTRUE){
    if(uartRecived){
      uartReceived--;
      str=bufferWIFI[indexWIFI_tx];
      indexWIFI_tx++;
      updateBufferWIFI();
      exito=1;
      *cmd=msg;
    }
    xSemaphoreGive(semWIFI);
  }
  return exito;
}