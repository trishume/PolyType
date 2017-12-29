// PolyType Firmware Main File
// By Tristan Hume

#include <Arduino.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <i2c_t3.h>

#include "Display.h"

#include "USBKeyboardOutput.h"
#include "TeensyMatrixSource.h"
#include "MCPMatrixSource.h"
#include "LayoutProcessor.h"
#include "MacroProcessor.h"
#include "CodeTransformer.h"
#include "ChordBundler.h"
#include "VeloHandler.h"
#include "SleepCounter.h"

Display disp;

// Das Pipeline Elements
TeensyMatrixSource teensySource;
MCPMatrixSource mcpSource;
LayoutProcessor layoutProc;
ChordBundler chordBundler;
VeloHandler veloHandler;
MacroProcessor macroProc;
CodeTransformer codeTrans;
SleepCounter sleepCounter;
USBKeyboardOutput usbOut;

unsigned long lastTick;

void connectPipeline() {
  teensySource.out = &layoutProc;
  mcpSource.out = &layoutProc;

  layoutProc.out = &chordBundler;
  layoutProc.disp = &disp;
  chordBundler.out = &macroProc;
  chordBundler.chordHandler = &veloHandler;
  veloHandler.out = &codeTrans;
  macroProc.out = &codeTrans;
  codeTrans.out = &sleepCounter;
  sleepCounter.out = &usbOut;
}

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  disp.setup();
  // Wire.begin(); // wake up I2C bus
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 1800000);

  teensySource.start();
  mcpSource.start();
  connectPipeline();

  layoutProc.setLayout(0);
  disp.render(false);

  lastTick = micros();
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
  disp.render(sleepCounter.pressedThisTick());
  // TODO add kicker pipeline step so this can be reenabled
  // if(sleepCounter > 0) sleepCounter--;

  // unsigned long now = micros();
  // unsigned long delta = now - lastTick;
  // lastTick = now;
  // if(sleepCounter.pressedThisTick()) {
  //   Serial.println(delta);
  // }

  sleepCounter.tick();

  // TODO implement debounce so this is unnecessary
  delay(2);
}
