#ifndef LAYOUTPROCESSOR_H__
#define LAYOUTPROCESSOR_H__

#include "PacketTypes.h"
#include "Pipe.h"

class LayoutProcessor : public Pipe<KeyMatrixEvent>
{
public:
  LayoutProcessor();
  Pipe<KeyNameEvent> *out;

  virtual void push(KeyMatrixEvent &event);

protected:
  int curLayout;
};

#endif
