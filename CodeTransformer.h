#ifndef CODETRANSFORMER_H__
#define CODETRANSFORMER_H__

#include "PacketTypes.h"
#include "Pipe.h"

class CodeTransformer : public Pipe<KeyNameEvent>
{
public:
  CodeTransformer() : out(0) {};
  Pipe<KeyCodeEvent> *out;

  virtual void push(const KeyNameEvent &event);

protected:
  int toCode(char group, char key);
};

#endif
