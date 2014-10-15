#include "SleepCounter.h"

#define SLEEP_COUNT 2000

SleepCounter::SleepCounter() : countDown(SLEEP_COUNT) {

}

void SleepCounter::kick() {
  countDown = SLEEP_COUNT;
}

void SleepCounter::push(KeyCodeEvent &ev) {
  kick();
  out->push(ev);
}

int SleepCounter::justWoke() {
  return countDown == SLEEP_COUNT;
}
