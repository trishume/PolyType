#ifndef __USB_KEYBOARD_H_
#define __USB_KEYBOARD_H_

#include "PacketTypes.h"
#include "Pipe.h"

#define MAX_KEYS_TRACK 32

class USBKeyboardOutput : public Pipe<KeyCodeEvent>
{
public:
  USBKeyboardOutput();
  ~USBKeyboardOutput() {};

  virtual void push(const KeyCodeEvent &ev);
  void commit();

private:
  bool changed;
  int numDown;
  int keysDown[MAX_KEYS_TRACK];

  void unpress(int index);
};

#endif
