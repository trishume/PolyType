#include "Display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_NeoPixel.h>

#include "Info.h"

#define LED_PIN   12
#define NUMPIXELS  3

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
static Adafruit_PCD8544 theOnlyDisplay = Adafruit_PCD8544(14, 16, 15);
const char *defaultHeader = "- PolyType -";

Display::Display() : dirty(true), layoutName(0) {
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

void Display::render() {
  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, LOW);
  // delay(3000);
  pixels.setPixelColor(0, pixels.Color(0,0,150));
  pixels.setPixelColor(1, pixels.Color(0,0,150));
  pixels.show();
  if(dirty == 0) return;
  dirty = 0;
  display->clearDisplay();
  if(!sleeping) {
    display->println(header);
    display->println(VERSION_STR);
    display->println(CODENAME_STR);
    if(layoutName != 0) {
      display->println("Layout:");
      display->println(layoutName);
    }
  }
  display->display();
}

void Display::setSleeping(bool state) {
  sleeping = state;
  dirty = 1;
}

void Display::setLayoutName(const char *name) {
  layoutName = name;
  dirty = 1;
}
