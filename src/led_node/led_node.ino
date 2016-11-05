/*
 C 2016 Marcel Ochsendorf
 marcel.ochsendorf@gmail.com
 https://github.com/RBEGamer
 24.07.2016
PCA LIB : https://github.com/TOppenhoff/PCA9685
WEBSERVER LIB : https://github.com/sirleech/Webduino
FLASH STORAGE : https://github.com/sebnil/DueFlashStorage
*/


#define ENABLE_WEBSERVER
#define USE_I2C_DISPLAY

#include <Wire.h>

#ifdef ENABLE_WEBSERVER
#include <SPI.h>
#include <Ethernet.h>
#include "WebServer.h"
#endif

 #include <elapsedMillis.h>

#ifdef USE_I2C_DISPLAY
 #include <LiquidCrystal_I2C.h>
 #endif
#include <PCA9685.h>


// PCWheelPosA9685 with default settings








void setup()
{
  // initialize serial for debug output
  Serial.begin(9600);
  // initialize TwoWire communication
  Wire.begin();




setup_pcas(); 

i2c_display_setup();

#ifdef ENABLE_WEBSERVER
webserver_setup();
#endif

}






void loop()
{


loop_pcas();
#ifdef ENABLE_WEBSERVER
webserver_loop();
#endif
i2c_display_loop();


}
