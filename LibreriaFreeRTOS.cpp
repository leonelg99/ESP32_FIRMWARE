#include "LibreriaFreeRTOS.h"


//Function Prototypes
static void conexionTask (void *);
static void uartTask (void *);
static void videoTask (void *);

//Function to Create all the main task
void createTasks(){
  
  //The Task for the camera is going to run in core 1
  xTaskCreatePinnedToCore(
    videoTask,
    "CAMARA",
    STACK_SIZE_CAM,
    NULL,
    1,
    NULL,
    1
  );

  //The Task for the server is going to run in core 0
  xTaskCreatePinnedToCore(
    conexionTask,
    "Server",
    STACK_SIZE_SERVER,
    NULL,
    1,
    NULL,
    0
  );

  //The Task for the UART is going to run in core 0
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
