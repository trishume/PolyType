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
  KeyCodeEvent(int c, KeyEventType typ) : BaseKeyEvent(typ), code(c) {}
  int code;
};

struct KeyNameEvent : public BaseKeyEvent
{
  KeyNameEvent(char group, char c, KeyEventType typ) : BaseKeyEvent(typ), group(group), key(c) {}
  char group; // ' ' = none, '.' = ascii, '~' = special
  char key;
};

struct KeyMatrixEvent : public BaseKeyEvent
{
  KeyMatrixEvent(int r, int c, KeyEventType typ) : BaseKeyEvent(typ), row(r), col(c) {}
  int row;
  int col;
};

// Chording Fanciness

// A bitmap where bits represent the corresponding letter of the alphabet
typedef unsigned velomap_t;
#define VMAP(c)  (1<<((c)-'A'))

struct ChordEvent
{
  ChordEvent(velomap_t left, velomap_t right, velomap_t mods) : l(left), r(right), mod(mods) {};
  velomap_t l;
  velomap_t r;
  velomap_t mod;
};

#endif
