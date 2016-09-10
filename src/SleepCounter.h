#ifndef SLEEPCOUNTER_H__
#define SLEEPCOUNTER_H__

#include "PacketTypes.h"
#include "Pipe.h"

#define SLEEP_COUNT 2000

class SleepCounter : public Pipe<KeyCodeEvent>
{
public:
  SleepCounter();
  Pipe<KeyCodeEvent> *out;

  int countDown;

  virtual void push(const KeyCodeEvent &event);
  void kick();
  void tick() {
    if(countDown > 0) countDown--;
  }

  int justWoke();
  bool shouldDoSleep() {
    return countDown == 1;
  }

  bool pressedThisTick() {
    return countDown == SLEEP_COUNT;
  }
};

#endif
