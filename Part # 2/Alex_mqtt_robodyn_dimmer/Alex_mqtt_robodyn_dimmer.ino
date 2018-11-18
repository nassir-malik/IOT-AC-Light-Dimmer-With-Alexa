#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include "hassio_mqtt.h"

boolean connectWifi();
boolean m_light_state = false;
//callback functions
void firstLightChanged(uint8_t brightness);
WiFiClient wifiClient;
//##################################################
//##################################################
// Change this!!
const char* ssid = "Entr your wifi router's name";
const char* password = "Enter wifi password";
//##################################################
//##################################################
boolean wifiConnected = false;
Espalexa espalexa;
EspalexaDevice* device3; //this is optional

void setup()
{
  Serial.begin(115200);
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    // Define your devices here. 
    espalexa.addDevice("office lights", firstLightChanged); //simplest definition, default state off
    espalexa.begin();
    hassioMqttSetup();
    
  } else
  {
    while (1) {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
 
void loop()
{
  if(WiFi.status() != WL_CONNECTED){
    connectWifi();
  }else{
   espalexa.loop();
   hassioMqttloop();
  }
  //@@@##############@@@@@@@@@@@@@###############@#@@@@@@@@@@@@@@@#
  yield();//Please don't chage or remove otherwise MCU will crash
  //@@@##############@@@@@@@@@@@@@###############@#@@@@@@@@@@@@@@@#
}

//our callback functions
void firstLightChanged(uint8_t brightness) {
    Serial.print("Device 1 changed to ");
    //EXAMPLE
    if (brightness) {
      Serial.print("ON, brightness ");
      Serial.println(brightness);
      m_light_state = true;
      publishLightState();
      publishLightBrightness(String(brightness));
    }
    else  {
      Serial.println("OFF");
      m_light_state = false;
      publishLightState();
    }
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
