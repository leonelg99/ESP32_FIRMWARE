#include "LibreriaWIFI.h"

#define ledPin 4

//Custom data types
struct Command
{
  String cmd; 
  String value;
  String value2;
};



//Network Parameters
const char* ssid = "ESP32";
const char* password = "esp32123";

//Seting router 
IPAddress staticIP(192, 168, 4, 2); // Cambia esta IP según tus necesidades
IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);


const char* HOST_IP;
const uint16_t HOST_PORT = 2000;

/*
const int   channel        = 10;                        // WiFi Channel number between 1 and 13
const bool  hide_SSID      = false;                     // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
const int   max_connection = 1;                         // Maximum simultaneous connected clients on the AP
*/

//COMANDOS
String message_aux; //main captured String 
String message;
String resp; //String, cmd o cam
Command CMD;

// Set web server port number to 80
WiFiServer server(80);

//Seput WiFi function
void startWiFi(){
  pinMode(ledPin,OUTPUT);
  WiFi.mode(WIFI_AP); //Optional
  WiFi.softAPConfig(local_ip, gateway, subnet);
  // You can remove the password parameter if you want the AP to be open.
  // a valid password must have more than 7 characters
  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation failed.");
    while(1){
      digitalWrite(ledPin,HIGH);
      delay(500);
      digitalWrite(ledPin,LOW);
      delay(500);
    };
  }
 
}

WiFiClient getClient(){
  WiFiClient client = server.available();
  if(client) return client;
}

void serverSetup(){
  //initSemaphores();
  server.begin();
  Serial.println("\nConnecting");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");

}
String MSG;
void serverExecute(){
  while(1){
    WiFiClient client= getClient();
    if(client){
      message_aux = client.readStringUntil('\n');  
      processMessage(message_aux, &resp, &message);
      if (resp.equals("cmd")){
        client.print("Eco: " + message);
        saveInBufferUART(message);
      }
      else {
        if(resp.equals("cam")){
          client.print("Es un comando: "+resp);
          //Crear tarea de CAMARA y ejecutar comando
          // processCamMessage(message, &command.cmd, &command.value, &command.value2);
          // executeCam();
        }
        else{
          if(resp.equals("get")){
              if(getMsg(&MSG)){
                digitalWrite(ledPin,HIGH);
                delay(500);
                digitalWrite(ledPin,LOW);
                delay(500);
                digitalWrite(ledPin,HIGH);
                delay(500);
                digitalWrite(ledPin,LOW);
                client.print(MSG);}
              else client.print("");
          } else client.print("ESP - Error: comando no valido.");
        }
     }
     client.stop();
    }
    if(getMsg(&MSG)){
      if (client.connect(HOST_IP, HOST_PORT,10)) {
        client.print("Hola, este es un mensaje desde el ESP32." + MSG);
        client.stop();
      }else saveInBufferWIFI(MSG);
    }

    vTaskDelay(2);
  }
} 


