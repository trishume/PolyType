/*
  DigitalReadSerial
 Reads a digital input on pin 2, prints the result to the serial monitor

 This example code is in the public domain.
 */

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "Wire.h"

#include "Display.h"

#include "USBKeyboardOutput.h"
#include "TeensyMatrixSource.h"
#include "MCPMatrixSource.h"
#include "LayoutProcessor.h"
#include "MacroProcessor.h"
#include "CodeTransformer.h"
#include "SleepCounter.h"

Display disp;

// Das Pipeline Elements
TeensyMatrixSource teensySource;
MCPMatrixSource mcpSource;
LayoutProcessor layoutProc;
MacroProcessor macroProc;
CodeTransformer codeTrans;
SleepCounter sleepCounter;
USBKeyboardOutput usbOut;

void connectPipeline() {
  teensySource.out = &layoutProc;
  mcpSource.out = &layoutProc;

  layoutProc.out = &macroProc;
  layoutProc.disp = &disp;
  macroProc.out = &codeTrans;
  codeTrans.out = &sleepCounter;
  sleepCounter.out = &usbOut;
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  disp.setup();
  Wire.begin(); // wake up I2C bus

  teensySource.start();
  mcpSource.start();
  connectPipeline();

  layoutProc.setLayout(0);
  disp.render();
}

// the loop routine runs over and over again forever:
void loop() {
  //Serial.println("updating");
  teensySource.update();
  mcpSource.update();
  usbOut.commit();

  if(sleepCounter.shouldDoSleep()) {
    Serial.println("sleep");
    disp.setSleeping(true);
  } else if(sleepCounter.justWoke()) {
    disp.setSleeping(false);
  }
  disp.render();
  // TODO add kicker pipeline step so this can be reenabled
  // if(sleepCounter > 0) sleepCounter--;
  sleepCounter.tick();

  delay(10);        // delay in between reads for stability
}
