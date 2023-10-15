#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "LibreriaWIFI.h"
#include "LibreriaUART.h"

#define STACK_SIZE_SERVER 10000 //4KB
#define STACK_SIZE_UART 10000   //4KB
#define STACK_SIZE_CAM 10000    //TO LOW FOR CAM

void createTasks(void);
