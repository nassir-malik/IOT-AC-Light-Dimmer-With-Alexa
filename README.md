# IOT-Light-Dimmer
IOT-Light-Dimmer


1): Setup Hassio

mqtt:
    password: "welcome"

light:
  - platform: mqtt
    name: 'Demo Light'
    state_topic: 'office/light/status'
    command_topic: "office/light/switch"
    optimistic: false

  - platform: mqtt
    name: "Office Lights"
    state_topic: "netmedias/office lights/status"
    command_topic: "netmedias/office lights/switch"
    brightness_state_topic: 'netmedias/office lights/brightness'
    brightness_command_topic: 'netmedias/office lights/brightness/set'
    qos: 0
    optimistic: false 
	
	
2): Downgrade ESP board in arduino ide


3): Download Esplexa and copy it into lib folder
https://github.com/Aircoookie/Espalexa


4): Download project update ssid and flash it
