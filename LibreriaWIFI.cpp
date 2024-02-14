#include "LibreriaWIFI.h"

#define ledPin 4

//Network Parameters
const char* ssid = "ESP32";
const char* password = "esp32123";

//Seting router 
IPAddress staticIP(192, 168, 4, 2); // Cambia esta IP según tus necesidades
IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

//Variables to process commands
String message_aux; //main captured String 
String message;
String resp; //String, cmd o cam

// Set web server port number to 80
WiFiServer server(80);

//This function is used to setup the ESP32 as an Access Point
void startWiFi(){
  int estado = 0;
  while(estado == 0){
    pinMode(ledPin,OUTPUT);
    WiFi.mode(WIFI_AP); //Optional
    WiFi.softAPConfig(local_ip, gateway, subnet);
    if (!WiFi.softAP(ssid, password)) {
      log_e("Soft AP creation failed.");
      delay(500);
    } else estado = 1;
  }
}

//This function is used to check if there is a client connected to the AP and return it
WiFiClient getClient(){
  WiFiClient client = server.available();
  if(client) return client;
}

void serverSetup(){
  server.begin();
}

String MSG="";

//This function is used to set the behaviour of the server 
//If there is data avaliable from a client, it reads it and process it
//It also checks who is the responsible to execute the command received (it could be the ESP32 or the MCU)
void serverExecute(){
  while(1){
    WiFiClient client= getClient();
    if(client){
      message_aux = client.readStringUntil('\n');  
      processMessage(message_aux, &resp, &message);
      if (resp.equals("cmd")){
        saveInBufferUART(message);
      }
      else {
        if(resp.equals("cam") || resp.equals("led")){
          if(resp.equals("cam"))
            changeResolution();
          else led();
        }
        else{
          if(resp.equals("get")){
              if(getMsg(&MSG)){
                client.print(MSG);
              } else client.print("");
          } else client.print("ESP - Error: comando no valido.");
        }
     }
     client.stop();
    }
    vTaskDelay(2);
  }
} 
