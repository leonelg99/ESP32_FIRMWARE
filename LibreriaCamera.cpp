#include "LibreriaCamera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"
#define PART_BOUNDARY "123456789000000000000987654321"
#define ledPin 4

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static bool led_state = false;

//Constants and variables for the camera
  // 10 –> UXGA(1600×1200)
  // 9 –> SXGA(1280×1024)
  // 8 –> XGA(1024×768)
  // 7 –> SVGA(800×600)
  // 6 –> VGA(640×480)
  // 5 — CIF(400×296)
  // 4 –> QVGA(320×240)
  // 3 –> HQVGA(240×176)
  // 0 –> QQVGA(160×120)
const int videoFramesizeUXGA = 10;
const int videoFramesizeSVGA = 7;
const int videoQualityLow = 12;
const int videoQualityHigh = 10;
int framesize = videoFramesizeSVGA;
httpd_handle_t stream_httpd = NULL;

void setupLedFlash(int);

static esp_err_t stream_handler(httpd_req_t *);
camera_config_t config;
//This functions is used for the setup of the camera connected to the ESP32-CAM
//It initializes the camera with the predefined configuration, such as pins, camera frame size and quality.
//Also, it handles any initialization errors.
void cameraSetup(){
  pinMode(ledPin,OUTPUT);

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 32000000;
  config.pixel_format = PIXFORMAT_JPEG; 
  
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 5;
    config.fb_count = 3;
    framesize = 7;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
    framesize = 7;
  }
  
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    //Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  
  cameraExecute();

}

//This function streams video data from the camera over HTTP. 
//It continuously captures frames, converts them to JPEG and sends them as an HTTP response. 
//It also handles errors and ensures proper content type settings. 
static esp_err_t stream_handler(httpd_req_t *req){
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  while(true){
    fb = esp_camera_fb_get();
    if (!fb) {
      //Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if(fb->width > 400){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            //Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    //Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
  }
  return res;
}

//This function is used to execute the camera using port 8020
void cameraExecute(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 8020;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
  
  //Serial.printf("Starting web server on port: '%d'\n", config.server_port);
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
  }
}

//This function is used to change the resolution of the camera during runtime
//At the moment, it needs revision
void changeResolution() {
    int int_framesize = videoFramesizeSVGA;
    int int_quality = videoQualityLow;
    if (framesize = videoFramesizeSVGA) {
      int_framesize = videoFramesizeUXGA;
      int_quality = videoQualityHigh;
    } else {
      int_framesize = videoFramesizeSVGA;
      int_quality = videoQualityLow;
    }
    // Detenemos la captura de la cámara
    esp_camera_fb_return(esp_camera_fb_get());
    delay(100);
    // Configuramos la resolución deseada
    config.frame_size = (framesize_t)int_framesize; // Puedes cambiar FRAMESIZE_QVGA a la resolución deseada
    config.jpeg_quality = int_quality;  // Calidad JPEG (de 0 a 63)

    // Inicializamos la cámara con la nueva configuración
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      saveInBufferUART("ERROR: CAMBIO DE RESOLUCION - REINICIANDO");
      cameraSetup();
    }
}

//This function is used to turn on/off a led
void led(){
  if(!led_state){
    led_state=true;
    digitalWrite(ledPin,HIGH);
  } else {
    led_state=false;
    digitalWrite(ledPin,LOW);
  }
}
