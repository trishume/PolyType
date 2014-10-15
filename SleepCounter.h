#ifndef SLEEPCOUNTER_H__
#define SLEEPCOUNTER_H__

#include "PacketTypes.h"
#include "Pipe.h"

class SleepCounter : public Pipe<KeyCodeEvent>
{
public:
  SleepCounter();
  Pipe<KeyCodeEvent> *out;

  int countDown;

  virtual void push(KeyCodeEvent &event);
  void kick();
  void tick() {
    if(countDown > 0) countDown--;
  }

  int justWoke();
  int shouldDoSleep() {
    return countDown == 1;
  }
};

#endif
