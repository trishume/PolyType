#include "LayoutProcessor.h"
#include "usb_keyboard.h"

static const int layout[6][14] = {
  {KEY_ESC         , KEY_ESC , KEY_2   , KEY_3   , KEY_4         , KEY_5        , KEY_TAB , KEY_TAB , KEY_6   , KEY_7          , KEY_8   , KEY_9     , KEY_MINUS   , KEY_RIGHT_BRACE},
  {KEY_RIGHT_BRACE , KEY_1   , KEY_W   , KEY_E   , KEY_R         , KEY_T        , KEY_TAB , KEY_TAB , KEY_Y   , KEY_U          , KEY_I   , KEY_O     , KEY_0       , KEY_BACKSLASH},
  {KEY_TAB         , KEY_Q   , KEY_S   , KEY_D   , KEY_F         , KEY_G        , KEY_TAB , KEY_TILDE,KEY_H   , KEY_J          , KEY_K   , KEY_L     , KEY_P       , KEY_LEFT_BRACE},
  {MODIFIERKEY_SHIFT,KEY_A   , KEY_X   , KEY_C   , KEY_V         , KEY_B        , KEY_TAB , KEY_TILDE,KEY_N   , KEY_M          , KEY_COMMA,KEY_PERIOD, KEY_SEMICOLON,KEY_QUOTE},
  {MODIFIERKEY_SHIFT,KEY_Z   , KEY_LEFT, KEY_RIGHT,MODIFIERKEY_GUI,KEY_BACKSPACE, KEY_ESC , KEY_ENTER,KEY_SPACE,MODIFIERKEY_CTRL, KEY_UP  , KEY_DOWN  , KEY_SLASH    ,MODIFIERKEY_SHIFT},
  {0, 0,MODIFIERKEY_SHIFT,0, MODIFIERKEY_ALT,KEY_MEDIA_PLAY_PAUSE, KEY_RIGHT_BRACE   , KEY_TAB , KEY_MINUS,KEY_EQUAL      , KEY_A   , 0         , MODIFIERKEY_GUI, 0},
};

LayoutProcessor::LayoutProcessor() : out(NULL) {}

void LayoutProcessor::push(KeyMatrixEvent &ev) {
  if(out == NULL) return;
  int code = layout[ev.row][ev.col];
  KeyCodeEvent newEvent(code, ev.type);
  out->push(newEvent);
}