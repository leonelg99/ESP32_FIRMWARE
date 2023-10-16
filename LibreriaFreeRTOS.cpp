#include "LibreriaFreeRTOS.h"


//Function Prototypes
static void conexionTask (void *);
static void uartTask (void *);
static void camTask (void *);

//String variable to recieve message by wifi 
String messageReceived;



//Function to Create all the main task
void createTasks(){
  xTaskCreatePinnedToCore(
    conexionTask,
    "Server",
    STACK_SIZE_SERVER,
    NULL,
    2,
    NULL,
    0
  );
  xTaskCreatePinnedToCore(
    uartTask,
    "UART",
    STACK_SIZE_UART,
    NULL,
    1,
    NULL,
    0
  );
  /*xTaskCreatePinnedToCore(
    camTask,
    "CAMARA",
    STACK_SIZE_CAM,
    NULL,
    1,
    NULL,
    1
  );*/
}

//Camara Task
/*static void conexionTask (void *parameter){
  cameraSetup();
  cameraStart();
}*/

//Server Task
static void conexionTask (void *parameter){
  serverSetup();
  serverExecute();
}

//UART Task
static void uartTask (void *parameter){
  uartSetup();
  uartStart();
}