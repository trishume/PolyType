#ifndef DISPLAY_H__
#define DISPLAY_H__

class Adafruit_PCD8544;
class Display
{
public:
  Display();

  void setup();
  void render(bool pressedThisTick);

  void setSleeping(bool sleep);
  void setLayoutName(const char *name);

  void cycleMode();

protected:
  Adafruit_PCD8544 *display;
  bool dirty;
  bool sleeping;
  const char *header;
  const char *layoutName;
  unsigned long lastLightUpdate;
  unsigned long lastKeyPress;
  unsigned long numEvents;

  int modeNum;

  void renderLights(unsigned long t);
};

#endif
