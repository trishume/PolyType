#include "USBKeyboardOutput.h"
#include <usb_keyboard.h>
#include <Arduino.h>

USBKeyboardOutput::USBKeyboardOutput() : changed(0), numDown(0) {
  for (int i = 0; i < MAX_KEYS_TRACK; ++i) {
    keysDown[i] = 0;
  }
}

void USBKeyboardOutput::push(const KeyCodeEvent &ev) {
  if(ev.code == 0) return;
  // commit control code
  if(ev.code == -1) {
    commit();
    return;
  }
  changed = 1;
  if (ev.type == KeyDown) {
    // 2 just to be safe, I'm not risking 1
    if(numDown == (MAX_KEYS_TRACK - 2)) return;
    // NOTE: we allow the same code to be down multiple times
    // to handle things like multiple different shift keys down at same time.
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
  for(int i = 0; i < 6; ++i)
    keyboard_keys[i] = 0;

  int normalKeysUsed = 0;
  uint16_t mediaKey = 0;
  for (int i = 0; i < numDown; ++i) {
    int code = keysDown[i];
    uint8_t msb = code >> 8;
    if(msb == 0xE0) {
      keyboard_modifier_keys = (keyboard_modifier_keys | code);
    } else if(msb == 0xE2 || (msb >= 0xE4 && msb <= 0xE7)) { // media key range
      mediaKey = code;
    } else if(msb == 0xF0) {
      if(normalKeysUsed < 6) {
        keyboard_keys[normalKeysUsed] = code;
        normalKeysUsed++;
      }
    } else {
      Serial.println("bad code");
    }
  }
  Keyboard.set_media(mediaKey);

  usb_keyboard_send();
  changed = 0;
}

void USBKeyboardOutput::unpress(int i) {
  keysDown[i] = keysDown[numDown - 1];
  keysDown[numDown - 1] = 0;
  numDown--;
}
