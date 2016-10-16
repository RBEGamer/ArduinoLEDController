# ArduinoLEDController
A 32 (or more) Channel pwm ethernet controller
WIP!!!!
A arduino based led coltroller, for rgb led strips! with webinterface, api, fading, i2c info display.
Uses the PCA9685 PWM Controller to drive ttl mosfets. you cant add more channels, simply add more PCS Controllers and edit the 
#define pca_count_pcas 2 in the sketch to your controller count.
