#include "messagesUtilities.h"
#define MAX 15

//Buffers WIFI y UART
String bufferWIFI[MAX];
int indexWIFI_rx=0;
int indexWIFI_tx=0;
int wifiFlag=0;

String bufferUART[MAX];
int indexUART_rx=0;
int indexUART_tx=0;
int uartFlag=0;

SemaphoreHandle_t semUART,semWIFI;

//Prototypes

static void updateBufferWIFI(void);
static void updateBufferUART(void);


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
  *message = message_aux.substring(ind1+1);
  
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


/*BUFFER UART DRIVERS*/

//Get command saved in bufferUART
bool getCommand(String *cmd){
  int exito=0;
  String msg;
  if(xSemaphoreTake(semUART,(TickType_t)5)==pdTRUE){
    if(uartFlag){
      uartFlag--;
      msg=bufferUART[indexUART_tx];
      bufferUART[indexUART_tx]="";
      indexUART_tx++;
      updateBufferUART();
      exito=1;
      *cmd=msg;
    }
    xSemaphoreGive(semUART);
  }
  return exito;
}

//Actualizar Indices de WIFI
static void updateBufferUART(){
  if(indexUART_rx>=MAX)
    indexUART_rx=0;
  if(indexUART_tx>=MAX)
    indexUART_tx=0;
}

//Save in Buffer a msg receive by WIFI to be send later to EDU-CIAA by UART
bool saveInBufferUART(String msg){
  int error = 1;
  if(xSemaphoreTake(semUART,(TickType_t)5)==pdTRUE){
    if(indexUART_rx<MAX){
      bufferUART[indexUART_rx]=msg;
      indexUART_rx++;
      uartFlag++;
      error=0;
    }
    xSemaphoreGive(semUART);
  }
  return error;
}


/*WIFI buffer drivers*/

//Get command saved in bufferWIFI if there is one
bool getMsg(String *msg){
  int exito=0;
  String str;
  if(xSemaphoreTake(semWIFI,(TickType_t)5)==pdTRUE){
    if(wifiFlag){
      wifiFlag--;
      str=bufferWIFI[indexWIFI_tx];
      indexWIFI_tx++;
      updateBufferWIFI();
      exito=1;
      *msg=str;
    }
    xSemaphoreGive(semWIFI);
  }
  return exito;
}

//Actualizar Indices de WIFI
static void updateBufferWIFI(){
  if(indexWIFI_rx>=MAX){
    indexWIFI_rx=0;
  }
  if(indexWIFI_tx>=MAX){
    indexWIFI_tx=0;
  }
}

//Save in Buffer a msg receive in UART to be send later by wifi
bool saveInBufferWIFI(String msg){
  int error = 1;
  if(xSemaphoreTake(semWIFI,(TickType_t)5)==pdTRUE){
    if(indexWIFI_rx<MAX){
      bufferWIFI[indexWIFI_rx]=msg;
      indexWIFI_rx++;
      wifiFlag++;
      //updateBufferWIFI();
      error=0;
    }
    xSemaphoreGive(semWIFI);
  }
  return error;
}