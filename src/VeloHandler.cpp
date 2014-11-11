#include "VeloHandler.h"

static const char *leftOrder  = "XZSBPGQCDTVWFJHKLMRNYAOEUI";
static const char *rightOrder = "AOEUIVLMRWNGKCXBPHDSFTYZQJ";
static const KeyNameEvent commitEvent('!','c',KeyDown);
static const KeyNameEvent shiftDownEvent('~','s',KeyDown);
static const KeyNameEvent shiftUpEvent('~','s',KeyUp);

#define NUM_COMBOS 11
static const velomap_t combos[NUM_COMBOS*2] = {
  VMAP('P')|VMAP('J'), VMAP('B'),
  VMAP('T')|VMAP('J'), VMAP('D'),
  VMAP('C')|VMAP('J'), VMAP('G'),
  VMAP('L')|VMAP('J'), VMAP('H'),
  VMAP('R')|VMAP('N'), VMAP('M'),
  VMAP('F')|VMAP('C'), VMAP('Q'),
  VMAP('L')|VMAP('R'), VMAP('V'),
  VMAP('F')|VMAP('J'), VMAP('V'),
  VMAP('H')|VMAP('J'), VMAP('W'),
  VMAP('N')|VMAP('J'), VMAP('W'),
  VMAP('Z')|VMAP('K'), VMAP('X'),
};

// For key bitmaps, numdown is same as popcnt
static int numDown(velomap_t x) {
  return __builtin_popcount(x);
}

VeloHandler::VeloHandler() {

}

void VeloHandler::push(const ChordEvent &ev) {
  // These are before the mapping because we want to be able to type the single char H
  // without it being caps by using the alternate mapping.
  if(ev.l == VMAP('H') && ev.r == 0) { // Capitalize
    capNext = true;
    return;
  } else if(ev.l == VMAP('Z') && ev.l == VMAP('Z')) { // Back-stroke
    // TODO handle repeat by remembering more than one stroke.
    for(int i = 0; i < lastStroke; ++i)
      pushKey('b', '~');
    lastStroke = 1;
    return;
  }

  velomap_t left = mapCombos(ev.l, combos, NUM_COMBOS);
  velomap_t right = mapCombos(ev.r, combos, NUM_COMBOS);
  int totalDown = (numDown(left)+numDown(right));
  lastStroke = 0;

  // insert space if the No-Space key isn't down
  bool doSpace = totalDown > 1;
  if(ev.mod & VMAP('N')) doSpace = !doSpace;
  if(doSpace) {
    pushKey(' ');
    lastStroke += 1;
  }

  // Special word 'a' shortcut
  if(left == VMAP('O') && right == VMAP('I')) {
    left = VMAP('A');
    right = 0;
  }

  lastStroke += transcribe(left, leftOrder, capNext);
  lastStroke += transcribe(right, rightOrder, false);
  capNext = false;
}

int VeloHandler::transcribe(velomap_t map, const char *order, bool cap) {
  int count = 0;
  if(cap) out->push(shiftDownEvent);
  while(*order != 0) {
    char c = *order;
    velomap_t mask = VMAP(c);
    if((map & mask) != 0) {
      pushKey(c);
      count++;
      if(cap) {
        out->push(shiftUpEvent);
        cap = false;
      }
    }
    order++;
  }
  return count;
}

void VeloHandler::pushKey(char key, char group) {
  KeyNameEvent ev(group,key,KeyDown);
  out->push(ev);
  out->push(commitEvent);
  ev.type = KeyUp;
  out->push(ev);
  out->push(commitEvent);
}

velomap_t VeloHandler::mapCombos(const velomap_t map, const velomap_t *combos, int numCombos) {
  velomap_t newMap = map;
  for(int i = 0; i < numCombos; ++i) {
    velomap_t mask = combos[i*2];
    if((newMap & mask) == mask) {
      newMap = newMap^mask;
      newMap |= combos[i*2+1];
    }
  }
  return newMap;
}
