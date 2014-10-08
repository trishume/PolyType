#include "USBKeyboardOutput.h"
#include "usb_keyboard.h"

USBKeyboardOutput::USBKeyboardOutput() : changed(0), numDown(0) {
  for (int i = 0; i < MAX_KEYS_TRACK; ++i) {
    keysDown[i] = 0;
  }
}

void USBKeyboardOutput::push(KeyCodeEvent &ev) {
  if(ev.code == 0) return;
  changed = 1;
  if (ev.type == KeyDown) {
    // -6 because there might be a pathological case
    // that segfaults the copy in commit()
    if(numDown == (MAX_KEYS_TRACK - 6)) return;
    // check if it's already down
    for (int i = 0; i < numDown; ++i) {
      if(keysDown[i] == ev.code) return;
    }

    keysDown[numDown] = ev.code;
    numDown++;
  } else if(ev.type == KeyUp) {
    for (int i = 0; i < numDown; ++i) {
      if(keysDown[i] == ev.code) {
        unpress(i);
        return;
      }
    }
  }
}

void USBKeyboardOutput::commit() {
  if(!changed) return;
  // init special exported variables
  keyboard_modifier_keys = 0;
  keyboard_media_keys = 0;
  for(int i = 0; i < 6; ++i)
    keyboard_keys[i] = 0;

  int normalKeysUsed = 0;
  for (int i = 0; i < numDown; ++i) {
    int code = keysDown[i];
    if((code & 0x8000) == 0x8000) {
      keyboard_modifier_keys = (keyboard_modifier_keys | code);
    } else if(code > 0 && code <= 0x80) { // media key range
      keyboard_media_keys = code;
    } else if(normalKeysUsed < 6) {
      keyboard_keys[normalKeysUsed] = code;
      normalKeysUsed++;
    }
  }

  usb_keyboard_send();
  changed = 0;
}

void USBKeyboardOutput::unpress(int i) {
  keysDown[i] = keysDown[numDown - 1];
  keysDown[numDown - 1] = 0;
  numDown--;
}
