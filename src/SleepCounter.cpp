#include "SleepCounter.h"

SleepCounter::SleepCounter() : countDown(SLEEP_COUNT) {

}

void SleepCounter::kick() {
  countDown = SLEEP_COUNT;
}

void SleepCounter::push(const KeyCodeEvent &ev) {
  if(ev.type == KeyDown) kick();
  out->push(ev);
}

int SleepCounter::justWoke() {
  return countDown == SLEEP_COUNT;
}
