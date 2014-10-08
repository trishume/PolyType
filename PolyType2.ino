/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Wire.h"

#include "USBKeyboardOutput.h"
#include "TeensyMatrixSource.h"
#include "MCPMatrixSource.h"
#include "LayoutProcessor.h"

#define SLEEP_COUNT 5000

const char *header = "- PolyType -";

int sleepCounter;
Adafruit_PCD8544 display = Adafruit_PCD8544(14, 16, 15);

// Das Pipeline Elements
TeensyMatrixSource teensySource;
MCPMatrixSource mcpSource;
LayoutProcessor layoutProc;
USBKeyboardOutput usbOut;

void debug(const char *str) {
  Serial.print(str);
  display.print(str);
  display.display();
}

void printScreenHeader() {
  display.clearDisplay();
  display.println(header);
  display.println("Version 0.0.3");
  display.println("Codename: Pipe");
  display.display();
}

void kick() {
  if(sleepCounter == 0) {
    printScreenHeader();
  }
  sleepCounter = SLEEP_COUNT;
}

void setupDisplay() {
  Serial.println("Setting up display");
  display.begin();
  display.setContrast(60);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
}

void connectPipeline() {
  teensySource.out = &layoutProc;
  mcpSource.out = &layoutProc;

  layoutProc.out = &usbOut;
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  setupDisplay();
  printScreenHeader();
  Wire.begin(); // wake up I2C bus

  teensySource.start();
  mcpSource.start();
  connectPipeline();

  sleepCounter = SLEEP_COUNT;
}

// the loop routine runs over and over again forever:
void loop() {
  //Serial.println("updating");
  teensySource.update();
  mcpSource.update();
  usbOut.commit();

  if(sleepCounter == 1) {
    Serial.println("sleep");
    display.clearDisplay();
    display.display();
  }
  // TODO add kicker pipeline step so this can be reenabled
  // if(sleepCounter > 0) sleepCounter--;

  delay(10);        // delay in between reads for stability
}
