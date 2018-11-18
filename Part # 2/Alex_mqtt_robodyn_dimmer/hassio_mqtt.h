#include <PubSubClient.h>
#include "robodyn_dimmer.h"
#define MQTT_VERSION MQTT_VERSION_3_1_1
// MQTT: ID, server IP, port, username and password
const PROGMEM char* MQTT_CLIENT_ID = "Office Lights";
const PROGMEM char* MQTT_SERVER_IP = "192.168.0.22";
const PROGMEM uint16_t MQTT_SERVER_PORT = 1883;
const PROGMEM char* MQTT_USER = "homeassistant";
const PROGMEM char* MQTT_PASSWORD = "welcome";

// MQTT: topics
const char* MQTT_LIGHT_STATE_TOPIC = "netmedias/office lights/status";
const char* MQTT_LIGHT_COMMAND_TOPIC = "netmedias/office lights/switch";

const char* brightness_state_topic = "netmedias/office lights/brightness";
const char* brightness_command_topic = "netmedias/office lights/brightness/set";

// payloads by default (on/off)
const char* LIGHT_ON = "ON";
const char* LIGHT_OFF = "OFF";

extern boolean m_light_state;// = false; // light is turned off by default

extern WiFiClient wifiClient;
PubSubClient client(wifiClient);

// function called to publish the state of the light (on/off)
void publishLightState() {
  if (m_light_state) {
    state=1;
    client.publish(MQTT_LIGHT_STATE_TOPIC, LIGHT_ON, true);
  } else {
    state=0;
    client.publish(MQTT_LIGHT_STATE_TOPIC, LIGHT_OFF, true);
  }
}

void publishLightBrightness(String val) {
    setBrightness1(val);
    client.publish(brightness_state_topic, val.c_str(), true);
}
// function called to turn on/off the light
void setLightState() {
  if (m_light_state) {
    state=1;
    Serial.println("INFO: Turn light on...");
  } else {
    state=0;
    Serial.println("INFO: Turn light off...");
  }
}
void setLightBrightness(int val) {
  setBrightness1(String(val));
  Serial.println(val);
  
}
// function called when a MQTT message arrived
void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }
  Serial.println(p_topic);
  Serial.println(payload);
  // handle message topic
  if (String(MQTT_LIGHT_COMMAND_TOPIC).equals(p_topic)) {
    // test if the payload is equal to "ON" or "OFF"
    if (payload.equals(String(LIGHT_ON))) {
      if (m_light_state != true) {
        m_light_state = true;
        setLightState();
        publishLightState();
      }
    } else if (payload.equals(String(LIGHT_OFF))) {
      if (m_light_state != false) {
        m_light_state = false;
        setLightState();
        publishLightState();
      }
    }
  }else if (String(brightness_command_topic).equals(p_topic)){
      Serial.println("Dimming ...");//payload.toInt()
      state=1;
      setLightBrightness(payload.toInt());
      publishLightBrightness(payload.c_str());
      client.publish(brightness_state_topic, payload.c_str(), true);
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("INFO: Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("INFO: connected");
      // Once connected, publish an announcement...
      publishLightState();
      // ... and resubscribe
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC);
      client.subscribe(brightness_command_topic);
    } else {
      Serial.print("ERROR: failed, rc=");
      Serial.print(client.state());
      Serial.println("DEBUG: try again in 5 seconds");
      yield();
    }
  }
}

void hassioMqttSetup() {
  dimmerSetup();
  setLightState();
  // init the MQTT connection
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);
  
}

void hassioMqttloop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
