#include <Arduino.h>
#include <esp.h>
//#include <Wifi.h>
#include <string.h>
#include "Adafruit_Si7021.h"
// #include "Adafruit_HTU21DF.h"
// #include <LiquidCrystal_I2C.h>

#define SCL 22  // Use default SCL for Huzzah32
#define SDA 23  // Use default SDA for Huzzah32
#define SensorI2CAddress 0x40  // This is the default address for the 7021 and HTU21DF Sensor.

// Support for 2 sensor boards:  Si7021(5-pin with unpined 3.3V), HTU21D (4-pin smaller board)
//#define HTU21DF true
#ifndef HTU21DF
Adafruit_Si7021 sensor = Adafruit_Si7021();
#else
Adafruit_HTU21DF sensor = Adafruit_HTU21DF();
#endif

// Sensor variables
float humidity = 0;
float temp = 0;

// Sleep Time (in µs) Need to figure out proper time
#define deepSleepTime 10 * 1000000 // 5s

// Timing variables
unsigned long msCurrent, msNext;  // for simple timing
int dotCounter;

void displayMeasurements();


void setup() {
  Serial.begin(115200);
  //Serial.begin(74800);
  // wait for serial port to open
  while (!Serial) {
    delay(10);
  }
  delay(2000);
  esp_sleep_enable_timer_wakeup(deepSleepTime); // 5 seconds
  Serial.println("I'm awake!");
  sensor.begin();
  displayMeasurements();
  
  // Prep the sleep routine
  Serial.print("I'm getting sleepy");
  msCurrent = millis();
  dotCounter = 0;
}

void loop() {
      msNext = millis();
      if(msNext - msCurrent > 100) {
        Serial.print('.');
        msCurrent = millis();
        dotCounter++;
      }
  if (dotCounter >= 50) {
    Serial.println();
    Serial.print("Goodnight!");
    esp_deep_sleep_start();
  }
}
void displayMeasurements() {
  // get the temp reading and print that
  Serial.print("T: ");
  Serial.print(sensor.readTemperature()*9/5+32);
  Serial.println("F");
  Serial.print("H: ");
  Serial.print(sensor.readHumidity());
  Serial.println("%");
}