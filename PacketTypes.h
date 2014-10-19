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

struct KeyNameEvent : public BaseKeyEvent
{
  KeyNameEvent(char group, char c, KeyEventType typ) : group(group), key(c), BaseKeyEvent(typ) {}
  char group; // ' ' = none, '.' = ascii, '~' = special
  char key;
};

struct KeyMatrixEvent : public BaseKeyEvent
{
  KeyMatrixEvent(int r, int c, KeyEventType typ) : row(r), col(c), BaseKeyEvent(typ) {}
  int row;
  int col;
};

#endif
