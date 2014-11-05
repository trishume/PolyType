#include "Display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

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
}

void Display::render() {
  if(dirty == 0) return;
  dirty = 0;

  display->clearDisplay();
  if(!sleeping) {
    display->println(header);
    display->println("Version 0.4.0");
    display->println("Codename Make");
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
