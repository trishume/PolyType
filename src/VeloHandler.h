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
  int transcribe(velomap_t map, const char *order, bool cap);
  void pushKey(char key, char group = '.');
  velomap_t mapCombos(const velomap_t map, const velomap_t *combos, int numCombos);

  bool capNext;
  int lastStroke;
};


#endif /* VELOHANDLER_H */
