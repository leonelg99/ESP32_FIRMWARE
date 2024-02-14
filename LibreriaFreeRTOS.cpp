#include "LibreriaFreeRTOS.h"


//Function Prototypes
static void conexionTask (void *);
static void uartTask (void *);
static void videoTask (void *);

//Function to Create all the main task
void createTasks(){
  xTaskCreatePinnedToCore(
    videoTask,
    "CAMARA",
    STACK_SIZE_CAM,
    NULL,
    1,
    NULL,
    1
  );
  xTaskCreatePinnedToCore(
    conexionTask,
    "Server",
    STACK_SIZE_SERVER,
    NULL,
    1,
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
  
}

//Camara Task
static void videoTask (void *parameter){
  //Serial.print("V");
  //cameraExecute();
}

//Server Task
static void conexionTask (void *parameter){
  serverExecute();
}

//UART Task
static void uartTask (void *parameter){
  uartStart();
}