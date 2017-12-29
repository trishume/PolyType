#include "Display.h"
#include "Info.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   12
#define NUMPIXELS  6
#define PIX_PER_SIDE 3
#define NUM_MODES 4

#define MODE_OFF 0
#define MODE_BLUE 1
#define MODE_WHEEL 2
#define MODE_LATENCY 3

static const uint8_t kMinBrightness = 30;
static const uint8_t kMaxBrightness = 70;
static const uint8_t kBrightnessRange = kMaxBrightness - kMinBrightness;

static Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
static Adafruit_PCD8544 theOnlyDisplay = Adafruit_PCD8544(14, 16, 15);
const char *defaultHeader = "- PolyType -";

static uint32_t colorWheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return Adafruit_NeoPixel::Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return Adafruit_NeoPixel::Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return Adafruit_NeoPixel::Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

Display::Display() : dirty(true), layoutName(0), lastLightUpdate(0), modeNum(0), numEvents(0) {
  display = &theOnlyDisplay;
  header = defaultHeader;
}

void Display::setup() {
  display->begin();
  display->setContrast(60);
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(BLACK);
  display->setCursor(0,0);
  pixels.begin();
  // pinMode(LED_PIN, OUTPUT);
}

void Display::render(bool pressedThisTick) {
  unsigned long t = millis();
  if(pressedThisTick) {
    lastKeyPress = t;
    numEvents += 1;
  }
  unsigned long timeSinceUpdate = t - lastLightUpdate;
  unsigned long timeBetweenUpdates = 30;
  if(modeNum == MODE_OFF) {
    timeBetweenUpdates = 4000;
  } else if(modeNum == MODE_LATENCY) {
    timeBetweenUpdates = 0;
  }

  if(!sleeping && timeSinceUpdate >= timeBetweenUpdates) {
    renderLights(t);
    lastLightUpdate = t;
  }

  if(dirty == 0) return;
  dirty = 0;

  display->clearDisplay();
  if(!sleeping) {
    display->println(header);
    display->println(VERSION_STR);
    display->print("Count:");
    display->println(numEvents);
    if(layoutName != 0) {
      display->println("Layout:");
      display->println(layoutName);
    }
  }
  display->display();
}

void Display::setSleeping(bool state) {
  if(state == sleeping) return;
  dirty = 1;
  sleeping = state;
  renderLights(millis());
}

void Display::setLayoutName(const char *name) {
  layoutName = name;
  dirty = 1;
}

void Display::cycleMode() {
  modeNum = (modeNum + 1) % NUM_MODES;
  renderLights(millis());
}

void Display::renderLights(unsigned long t) {
  if(sleeping) {
    for(int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
    pixels.show();
    return;
  }

  unsigned long keyFade = (t - lastKeyPress)>>4;
  if(keyFade >= kBrightnessRange) {
    pixels.setBrightness(kMinBrightness);
  } else {
    pixels.setBrightness(kMaxBrightness-keyFade);
  }

  if(modeNum == MODE_LATENCY) {
    pixels.setBrightness(200);
  }

  uint32_t side[PIX_PER_SIDE];
  unsigned long wheelPos = t >> 5;
  for(unsigned i = 0; i < PIX_PER_SIDE; ++i) {
    if(modeNum == MODE_OFF) {
      side[i] = Adafruit_NeoPixel::Color(0,0,0);
    } else if(modeNum == MODE_BLUE) {
      side[i] = Adafruit_NeoPixel::Color(0,0,100);
    } else if(modeNum == MODE_WHEEL) {
      side[i] = colorWheel((wheelPos+(i*12)) % 256);
    } else if(modeNum == MODE_LATENCY) {
      side[i] = colorWheel(((numEvents % 3) * 83) % 256);
    }
  }

  for(int i = 0; i < PIX_PER_SIDE; i++) {
    pixels.setPixelColor(i, side[i]);
    pixels.setPixelColor(NUMPIXELS-i, side[i]);
  }
  pixels.show();
}
