#include "MacroProcessor.h"
#include "Arduino.h"

#define NUM_MACROS 2

static const char *macroTable[] = {
  "/.1/.2/.3",
  "+~s/.P/.S/.P-~s"
};

static const KeyNameEvent commitEvent('!','c',KeyDown);

void MacroProcessor::push(const KeyNameEvent &ev) {
  if(out == 0) return;

  if(ev.group == '>' && ev.type == KeyDown) {
    doMacro(ev.key);
    return;
  }

  out->push(ev);
}

void MacroProcessor::doMacro(char key) {
  int num = key - '0';
  if(num < 0 || num >= NUM_MACROS) return;
  const char *macro = macroTable[num];
  playSeq(macro);
}

void MacroProcessor::playSeq(const char *seq) {
  const char *c = seq;
  while(*c != 0) {
    if(*c == '/' || *c == '+' || *c == '-') {
      pushKey(*c, *(c+1), *(c+2));
      c+=2;
    } else if(*c >= 'a' && *c <= 'z') {
      pushKey('/','.',*c - ('a'-'A'));
    }
    ++c;
  }
}

void MacroProcessor::pushKey(char mode, char group, char key) {
  KeyNameEvent ev(group,key,(mode == '-') ? KeyUp : KeyDown);
  out->push(ev);
  out->push(commitEvent);
  // now release the key if this is a press
  if(mode == '/') {
    ev.type = KeyUp;
    out->push(ev);
    out->push(commitEvent);
  }
}
