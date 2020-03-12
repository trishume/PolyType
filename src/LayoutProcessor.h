#ifndef LAYOUTPROCESSOR_H__
#define LAYOUTPROCESSOR_H__

#include "PacketTypes.h"
#include "Pipe.h"
#include "Display.h"

#define INITIAL_LAYOUT 2

class LayoutProcessor : public Pipe<KeyMatrixEvent>
{
public:
  LayoutProcessor();
  Pipe<KeyNameEvent> *out;
  Display *disp;

  virtual void push(const KeyMatrixEvent &event);

  void setLayout(int layoutNum);
protected:
  int curLayout;
  int prevLayout;

  void layoutControl(char key);
};

#endif
