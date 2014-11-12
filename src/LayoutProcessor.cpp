#include "LayoutProcessor.h"
#include "usb_keyboard.h"

#define NUM_LAYOUTS 4
#define LAYOUT_ROWS 6
#define LAYOUT_COLS 14

// 29 because 14 keys * 2 chars + 1 null term, 7 because 6 cols + name
// ~e=enter, ~s=shift, ~a=command, ~o=option, ~c=control, ~h=hyper,
// ~x=escape, ~t=tab, ~p=play, ~l=left, ~r=right, ~u=up, ~d=down, ~b=backspace
static const char layouts[NUM_LAYOUTS][LAYOUT_ROWS+1][LAYOUT_COLS*2+1] = {
  {
    "~x>0.2.3.4.5.6.5.6.7.8.9_d_+",
    ".].1.W.E.R.T~f~g.Y.U.I.O.0.\\",
    "~t.Q.S.D.F.G    .H.J.K.L.P.[",
    "~s.A.X.C.V.B~t.`.N.M.,...;.'",
    "~s.Z~l~r~a~b~x~e. ~c~u~d./~s",
    "    ~s  ~o~p~j~k.-.=    ~h  ",
    "q;Soft Dvorak",
  },
  {
    "_________________________q__",
    ".=__.,...P.Y____.F.G.C.R____",
    "__.'.O.E.U.I    .D.H.T.N.L./",
    "__.A.Q.J.K.X____.B.M.W.V.S.-",
    "__.;____________________.Z__",
    "    __  ________.[.]    __  ",
    "d;Hard Dvorak",
  },
  {
    "_________________________q__",
    "__.,[P[K[I..____.,]O]K]P____",
    "[Z[F[T[J[O[U    ]U]I]J]T]F]Z",
    "[Z[S[C[R[E[A____]A]E]R]C]S]Z",
    "[Z[F____[L[N[Y]Y]N]L____]F]Z",
    "    [H  ~a~b~x~e. ~s    &N  ",
    "p;VeloType",
  },
  {
    "_________________________q__",
    "______~u____________.W______",
    "____~l~d~r__    __.A.S.D.___",
    "____________________________",
    "____________________________",
    "    __  ____________    __  ",
    "a;ARROWS+WASD",
  }
};

LayoutProcessor::LayoutProcessor() : out(0), curLayout(0) {}

void LayoutProcessor::push(const KeyMatrixEvent &ev) {
  if(out == 0) return;

  char group = layouts[curLayout][ev.row][ev.col*2];
  char key = layouts[curLayout][ev.row][ev.col*2+1];

  if(group == '_' && key == '_') { // base layer inherit
    group = layouts[0][ev.row][ev.col*2];
    key = layouts[0][ev.row][ev.col*2+1];
  }
  if(group == '_' && ev.type == KeyDown) { // handle layout control keys here
    layoutControl(key);
    return;
  }

  KeyNameEvent newEvent(group, key, ev.type);

  out->push(newEvent);
}

void LayoutProcessor::setLayout(int num) {
  num = num % NUM_LAYOUTS;
  curLayout = num;
  if(disp) {
    const char *name = layouts[curLayout][LAYOUT_ROWS]+2;
    disp->setLayoutName(name);
  }
}

void LayoutProcessor::layoutControl(char key) {
  switch(key) {
  case '+': // cycle to next layout
    setLayout(curLayout+1);
    return;
  case '-':
    setLayout(curLayout-1);
    return;
  }
  // lookup layout
  for (int i = 0; i < NUM_LAYOUTS; ++i) {
    if(layouts[i][LAYOUT_ROWS][0] == key) {
      setLayout(i);
      return;
    }
  }
}
