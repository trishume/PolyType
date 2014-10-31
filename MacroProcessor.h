#ifndef _MACROPROCESSOR_H_
#define _MACROPROCESSOR_H_

#include "PacketTypes.h"
#include "Pipe.h"

class MacroProcessor : public Pipe<KeyNameEvent>
{
public:
  MacroProcessor() : out(0) {};
  Pipe<KeyNameEvent> *out;

  virtual void push(const KeyNameEvent &event);
protected:
  void doMacro(char key);
  void playSeq(const char *seq);
  void pushKey(char mode, char group, char key);
};


#endif /* _MACROPROCESSOR_H_ */
