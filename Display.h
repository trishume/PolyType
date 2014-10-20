#ifndef DISPLAY_H__
#define DISPLAY_H__

class Adafruit_PCD8544;
class Display
{
public:
  Display();

  void setup();
  void render();

  void setSleeping(bool sleep);

protected:
  Adafruit_PCD8544 *display;
  bool dirty;
  bool sleeping;
  const char *header;
};

#endif
