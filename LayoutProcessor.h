#ifndef LAYOUTPROCESSOR_H__
#define LAYOUTPROCESSOR_H__

#include "PacketTypes.h"
#include "Pipe.h"
#include "Display.h"

class LayoutProcessor : public Pipe<KeyMatrixEvent>
{
public:
  LayoutProcessor();
  Pipe<KeyNameEvent> *out;
  Display *disp;

  virtual void push(KeyMatrixEvent &event);

  void setLayout(int layoutNum);
protected:
  int curLayout;

  void layoutControl(char key);
};

#endif
