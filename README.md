# ArduinoLEDController

### This is a massive PWM-Channel controller, to control multible RGB(W)-LED-Strips
Its perfect for combining normal led strips with other light equipment.
The api is fast so you can combine it with dmx equipment to make a lightshow.
A arduino based led coltroller, for rgb led strips! with webinterface, api, fading, i2c info display.
Uses the PCA9685 PWM Controller to drive ttl mosfets. you cant add more channels by adding more PCA9685 Boards

# PARTS
The listed parts are for 32 seperate channels, but you can add up to 64*16 channel to it by adding more PCA9685 boards and mosfets.

* Arduino UNO/NANO/MEGA/..
* 1-63 PCA9685 Breakout-Boards
* for each PCA-Board 16 320Ohm resistors
* for each PCA-Board 16 LogicLevel-Mosfets like the IRF520
* Arduino Ethernet-Shield with the WIZ Chip
* 12V 20A Powersupply, if you are using 12V LED Strips
* luster terminals or solder the leds directly on to the mosfets
* woodplate to mount the stuff
* [OPTIONAL] 5V relais to switch on the power supply
* [OPTIONAL] 16x4 I2C DotMatrix Display

# TOOLS
* soldering equipment

# SETUP HARDWARE
* put the ethernet shield on top of the arduino headers
* connect the lcs display to the I2C Bus of the arduino (and `VCC`, `GROUND)
* connect all PCA9685 Boards to the I2C Bus of the Arduino (and `VCC`, `GROUND`)
* connect each resistor from a channel output to `VCC of each channel of the PCA Boards
* connect each channel of the PCA-Boars to the `Base of MOSFET
* connect the source pin of each MOSFET to `GND`
* connect the drain pin of each MOSFET to a luster terminal
* connect the input of relais to pin D2 of the arduino (you can change it with `PIN_CONFIG_LED_RELAIS 2`)
* connect the rail between the `12V-VCC` of your power supply and the `12V IN of our led strips

# SETUP SOFTWARE
* download the latest Arduino-IDE
* download the PCA9685 lib (https://github.com/TOppenhoff/PCA9685)
* download the Webserver lib (https://github.com/sirleech/Webduino)
* download the FlashStorage lib (https://github.com/sebnil/DueFlashStorage)
* open the pc_controller.ino and edit the `#define pca_count_pcas ` to your pca amount 1-63
* upload the sketch to the arduino

### NOTE FOR ARDUINO DUE
you only need the FlashStorage lib if you are using an Arduino DUE

# USAGE
* connect a ehternet cable to the ehternetshield
* open our browser and goto to the ip, these will show on the lcd display at startup
* now you can set the channels
* you can use a api, please see the description on the webinterface


## IMAGE OF A 32 CHANNEL SETUP

### 32 Channel controller with alu frame as heatsink
![alt tag](https://raw.githubusercontent.com/RBEGamer/ArduinoLEDController/master/images/32channel.jpeg)
