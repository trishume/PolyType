#ifndef VELOHANDLER_H
#define VELOHANDLER_H

#include "PacketTypes.h"
#include "Pipe.h"

class VeloHandler : public Pipe<ChordEvent>
{
  public:
  VeloHandler();

  Pipe<KeyNameEvent> *out;

  virtual void push(const ChordEvent &ev);
  protected:
  void transcribe(velomap_t map, const char *order);
  void pushKey(char key);
};


#endif /* VELOHANDLER_H */
