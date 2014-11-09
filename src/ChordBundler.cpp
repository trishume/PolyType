#include "ChordBundler.h"

ChordBundler::ChordBundler() : down(0), left(0), right(0) {

}

void ChordBundler::push(const KeyNameEvent &ev) {
  if(ev.group != '[' && ev.group != ']' && ev.group != '&') {
    out->push(ev);
    return;
  }

  if(ev.type == KeyDown) {
    down++;
    switch(ev.group) {
    case '[':
      left |= VMAP(ev.key);
      break;
    case ']':
      right |= VMAP(ev.key);
      break;
    case '&':
      mods |= VMAP(ev.key);
    }
  } else if(ev.type == KeyUp) {
    down--;
  }

  // commit the chord
  if(down == 0) {
    ChordEvent chord(left, right, mods);
    chordHandler->push(chord);
    left = 0;
    right = 0;
    mods = 0;
  }
}
