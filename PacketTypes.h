#ifndef PACKETTYPES_H__
#define PACKETTYPES_H__

enum KeyEventType {KeyDown, KeyUp};

struct BaseKeyEvent
{
  BaseKeyEvent(KeyEventType typ) : type(typ) {}
  KeyEventType type;
};

struct KeyCodeEvent : public BaseKeyEvent
{
  KeyCodeEvent(int c, KeyEventType typ) : code(c), BaseKeyEvent(typ) {}
  int code;
};

#endif
