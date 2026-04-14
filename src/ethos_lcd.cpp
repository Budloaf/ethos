#include <Arduino.h>
#include <esp.h>
#include <string.h>
#include "Adafruit_Si7021.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define SCL 22  // Use default SCL for ESP32
#define SDA 23  // Use default SDA for ESP32
#define SensorI2CAddress 0x40  // This is the default address for the 7021 Sensor.
Adafruit_Si7021 sensor = Adafruit_Si7021();
#define LCDI2CAddress 0x3F  // This is the address for the LCD display (can vary).
LiquidCrystal_I2C lcd(LCDI2CAddress, 16, 2);  // Setup for 16 columns and 2 rows.

// Sensor variables
float humidity = 0;
float temp = 0;

// Timing variables
unsigned long msCurrent, msLast;  // for capturing MS for timing
#define msUpdateInt 5000          // Update the readings every msUpdateInt

int bye = 0;
char choice = '?';

void updateDisplay();
void displayEthosMenu();
void ethosInterface();
char typedKey();

void setup() {
	// Setup a serial terminal for debugging
  Serial.begin(115200);
  // wait for serial port to open
  while (!Serial) {
    delay(10);
  }
  delay(2000);
  Serial.println("\nSerial ready!");

  // Initialize the LCD
	lcd.init();
	// Turn on the blacklight and print a message.
	lcd.backlight();
	lcd.setCursor(0,0);
	//lcd.print("Test2");
  delay(2000);

  // Timing -  One way to track time for 5s delay.
  msCurrent = 0;
  msLast = msCurrent;
  displayEthosMenu();
}

void loop() {
  msCurrent = millis(); // it has been this many ms since the program began
  if(!bye) {
    ethosInterface();
  }
  if (msCurrent - msLast > msUpdateInt) {
    msLast = msCurrent;
    updateDisplay();
  }
  delay(5);
}

void updateDisplay() {
  lcd.setCursor(0,0);
  lcd.print("Temp (F): ");
  // get the temp reading and print that
  lcd.print(sensor.readTemperature()*9/5+32);
  lcd.setCursor(14,0); // overwriting hundredths place decimal. Not elegant. fix later if time permits.
  lcd.print((char)223); // degrees (°)
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  // get the humidity reading and print that
  lcd.print(sensor.readHumidity());
  lcd.setCursor(14,1); // overwriting hundredths place decimal. Not elegant. fix later if time permits.
  lcd.print("%");
}

void displayEthosMenu() {
  Serial.println("Welcome to ETHOS\nThe Electronic Temperature & Humidity Observation System\n\nYour Options:");
  Serial.println("  T  Request temperature");
  Serial.println("  H  Request relative humidity");
  Serial.println("  B  Request both temperature and relative humidity");
  Serial.println("  ?  Help (shows this menu)");
  Serial.print  ("  X  Exit\n\nChoice? ");
}

char typedKey() {
  char key;
  if(Serial.available() > 0) {
	  key = Serial.read();
    Serial.print(key);
    return key;
	}
  return 0;
}

void ethosInterface() {
  char key = typedKey();
  if (key != '\n' && key != '\r' && key != 0) {
    choice = key;
  }
  key = typedKey();
  if (key == '\n' || key == '\r') { //
    switch(choice) {
      case 0:
        break;
      case 't':
      case 'T': // temperature request!
        Serial.print("Temperature: ");
        Serial.print((int)(sensor.readTemperature()*9/5+32));
        Serial.print(" degrees F\n\nChoice? ");
        break;
      case 'h':
      case 'H': // humidity request!
        Serial.print("Relative humidity: ");
        Serial.print((int)sensor.readHumidity());
        Serial.print("%\n\nChoice? ");
        break;
      case 'b':
      case 'B': // temperature & humidity request!
        Serial.print("Temperature: ");
        Serial.print((int)(sensor.readTemperature()*9/5+32));
        Serial.print(" degrees F, Relative humidity: ");
        Serial.print((int)sensor.readHumidity());
        Serial.print("%\n\nChoice? ");
        break;
      case 'x':
      case 'X': // [EXIT]
        Serial.print("Goodbye!");
        bye = 1;
        break;
      default: // HELP
        displayEthosMenu();
        break;
    }
  }
}
