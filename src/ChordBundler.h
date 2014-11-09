#ifndef CHORDBUNDLER_H
#define CHORDBUNDLER_H

#include "PacketTypes.h"
#include "Pipe.h"

class ChordBundler : public Pipe<KeyNameEvent>
{
  public:
  ChordBundler();
  Pipe<KeyNameEvent> *out;
  Pipe<ChordEvent> *chordHandler;

  virtual void push(const KeyNameEvent &ev);

  protected:
  int down;
  velomap_t left;
  velomap_t right;
  velomap_t mods;
};


#endif /* CHORDBUNDLER_H */
