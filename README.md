# IOT-Light-Dimmer
IOT-Light-Dimmer

![alt text](https://github.com/nassir-malik/IOT-Light-Dimmer/blob/master/drawing.jpg)

##Part List

Triac BTA16 600B

Optocoupler 4n25

Triac Output Optocoupler MOC3021 

Bridge Rectifier diode d11510s

120k Ohms 5%

120k Ohms 5%

1k Ohms 5%

Resistor 331

Resistor 471

Resistor 103

LED

YouTube Tutorial Part # 1

https://www.youtube.com/watch?v=eFeOSiL-IBQ

YouTube Tutorial Part # 2

https://www.youtube.com/watch?v=uC9vyMdiJKQ

1): Setup Hassio

mqtt:

    password: "welcome"

light:

  \- platform: mqtt
  
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


4): Download project update ssid and password and flash it to NodeMcu
