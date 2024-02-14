#include <Arduino.h>
//#include "LibreriaWIFI.h"
//void initSemaphores();

void initSemaphores(void);
void processMessage(String, String *, String *);
bool saveInBufferUART(String);
bool saveInBufferWIFI(String);
bool getCommand(String *);
bool getMsg(String *);
void initBuffers(void);
