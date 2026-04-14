## ETHOS - Electronic Temperature & Humidity Observation System
ETHOS gathers temperature and relative humidity information from an Si7021 I2C-based temperature-humidity sensor and continuously displays the results on an I2C-based 1602A 16x2 LCD display. 
ETHOS provides temperature and humidity information to remote users interactively via RS232. 

Hardware: ESP32, Si7021 Sensor or HTU21DF Sensor, LCD-1602A 16x2 LCD display (optional), jumper wires
Software: PlatformIO (VScode extension)

There are two versions:
- ethos_lcd - displays results to LCD display and serial terminal 
- ethos_serial - displays results only to serial terminal
