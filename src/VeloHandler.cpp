#include "VeloHandler.h"

static const char *leftOrder  = "XZSBPGQCDTVWFJHKLMRNYAOEUI";
static const char *rightOrder = "AOEUIVLMRWNGKCXBPHDSFTYZQJ";
static const KeyNameEvent commitEvent('!','c',KeyDown);

// For key bitmaps, numdown is same as popcnt
static int numDown(velomap_t x) {
  return __builtin_popcount(x);
}

VeloHandler::VeloHandler() {

}

void VeloHandler::push(const ChordEvent &ev) {
  int totalDown = (numDown(ev.l)+numDown(ev.r));
  // insert space if the No-Space key isn't down
  bool doSpace = totalDown > 1;
  if(ev.mod & VMAP('N')) doSpace = !doSpace;
  if(doSpace) pushKey(' ');

  transcribe(ev.l, leftOrder);
  transcribe(ev.r, rightOrder);
}

void VeloHandler::transcribe(velomap_t map, const char *order) {
  while(*order != 0) {
    char c = *order;
    velomap_t mask = VMAP(c);
    if((map & mask) != 0) {
      pushKey(c);
    }
    order++;
  }
}

void VeloHandler::pushKey(char key) {
  KeyNameEvent ev('.',key,KeyDown);
  out->push(ev);
  out->push(commitEvent);
  ev.type = KeyUp;
  out->push(ev);
  out->push(commitEvent);
}
