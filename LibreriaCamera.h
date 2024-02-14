#include <Arduino.h>
#include "esp_camera.h"
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#include <WiFi.h>
#include "messagesUtilities.h"

void cameraSetup(void);
void cameraExecute(void);
void changeResolution(void);
void led(void);
