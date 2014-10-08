#ifndef LAYOUTPROCESSOR_H__
#define LAYOUTPROCESSOR_H__

#include "PacketTypes.h"
#include "Pipe.h"

class LayoutProcessor : public Pipe<KeyMatrixEvent>
{
public:
  LayoutProcessor();
  Pipe<KeyCodeEvent> *out;

  virtual void push(KeyMatrixEvent &event);
};

#endif
