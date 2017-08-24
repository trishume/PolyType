#include "CodeTransformer.h"
#include "usb_keyboard.h"

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

static const int asciiKeyCodes[65] = {
  KEY_SPACE, //
  0, // !
  0, // "
  0, // #
  0, // $
  0, // %
  0, // &
  KEY_QUOTE, // '
  0, // (
  0, // )
  0, // *
  0, // +
  KEY_COMMA, // ,
  KEY_MINUS, // -
  KEY_PERIOD, // .
  KEY_SLASH, // /
  KEY_0, // 0
  KEY_1, // 1
  KEY_2, // 2
  KEY_3, // 3
  KEY_4, // 4
  KEY_5, // 5
  KEY_6, // 6
  KEY_7, // 7
  KEY_8, // 8
  KEY_9, // 9
  0, // :
  KEY_SEMICOLON, // ;
  0, // <
  KEY_EQUAL, // =
  0, // >
  0, // ?
  0, // @
  KEY_A, // A
  KEY_B, // B
  KEY_C, // C
  KEY_D, // D
  KEY_E, // E
  KEY_F, // F
  KEY_G, // G
  KEY_H, // H
  KEY_I, // I
  KEY_J, // J
  KEY_K, // K
  KEY_L, // L
  KEY_M, // M
  KEY_N, // N
  KEY_O, // O
  KEY_P, // P
  KEY_Q, // Q
  KEY_R, // R
  KEY_S, // S
  KEY_T, // T
  KEY_U, // U
  KEY_V, // V
  KEY_W, // W
  KEY_X, // X
  KEY_Y, // Y
  KEY_Z, // Z
  KEY_LEFT_BRACE, // [
  KEY_BACKSLASH, // backslash
  KEY_RIGHT_BRACE, // ]
  0, // ^
  0, // _
  KEY_TILDE, // `
};

static const int specialKeyCodes[26] = {
  MODIFIERKEY_GUI, // a
  KEY_BACKSPACE, // b
  MODIFIERKEY_CTRL, // c
  KEY_DOWN, // d
  KEY_ENTER, // e
  KEY_HOME, // f
  KEY_END, // g
  MODIFIERKEY_CTRL | MODIFIERKEY_GUI | MODIFIERKEY_ALT | MODIFIERKEY_SHIFT, // h
  KEY_CAPS_LOCK, // i
  KEY_PAGE_DOWN, // j
  KEY_PAGE_UP, // k
  KEY_LEFT, // l
  0, // m
  0, // n
  MODIFIERKEY_ALT, // o
  KEY_MEDIA_PLAY_PAUSE, // p
  0, // q
  KEY_RIGHT, // r
  MODIFIERKEY_SHIFT, // s
  KEY_TAB, // t
  KEY_UP, // u
  0, // v
  0, // w
  KEY_ESC, // x
  0, // y
  0, // z
};

static const int fnKeyCodes[10] = {
  KEY_F10,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
};

void CodeTransformer::push(const KeyNameEvent &ev) {
  if(out == 0) return;
  int code = toCode(ev.group, ev.key);
  if(code == 0) return;

  KeyCodeEvent newEvent(code, ev.type);
  out->push(newEvent);
}

int CodeTransformer::toCode(char group, char key) {
  switch (group) {
  case '.':
    if(key < ' ' || key > '`') return 0; // bad layout
    return asciiKeyCodes[key-' '];
  case '~':
    if(key < 'a' || key > 'z') return 0; // bad layout
    return specialKeyCodes[key-'a'];
  case 'f':
    if(key < '0' || key > '9') return 0; // bad layout
    return fnKeyCodes[key-'0'];
  case '!':
    if(key == 'c') return -1; // commit
    if(key == 'r') {
      CPU_RESTART
    }
    return 0;
  }
  return 0; // unknown type
}
