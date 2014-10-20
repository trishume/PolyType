#include "LayoutProcessor.h"
#include "usb_keyboard.h"

// 29 because 14 keys * 2 chars + 1 null term, 7 because 6 cols + name
// ~e=enter, ~s=shift, ~a=command, ~o=option, ~c=control, ~h=hyper,
// ~x=escape, ~t=tab, ~p=play, ~l=left, ~r=right, ~u=up, ~d=down, ~b=backspace
static const char layouts[1][7][29] = {
  {
    "~x. .2.3.4.5. . .6.7.8.9. . ",
    ".].1.W.E.R.T~f~g.Y.U.I.O.0.\\",
    "~t.Q.S.D.F.G    .H.J.K.L.P.[",
    "~s.A.X.C.V.B~t.`.N.M.,...;.'",
    "~s.Z~l~r~a~b~x~e. ~c~u~d./~s",
    "    ~s  ~o~p~j~k.-.=    ~h  ",
    "q;Software Dvorak;          ",
  }
};

LayoutProcessor::LayoutProcessor() : out(NULL), curLayout(0) {}

void LayoutProcessor::push(KeyMatrixEvent &ev) {
  if(out == NULL) return;

  char group = layouts[curLayout][ev.row][ev.col*2];
  char key = layouts[curLayout][ev.row][ev.col*2+1];
  KeyNameEvent newEvent(group, key, ev.type);

  out->push(newEvent);
}
