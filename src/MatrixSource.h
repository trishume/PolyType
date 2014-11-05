#ifndef MATRIXSOURCE_H__
#define MATRIXSOURCE_H__

#include "PacketTypes.h"
#include "Pipe.h"

#define MAT_ROWS 6
#define MAT_COLS 7

class MatrixSource
{
public:
  MatrixSource(int offset);
  Pipe<KeyMatrixEvent> *out;
  int colOffset;

  virtual void update() = 0;
protected:
  int matrix[MAT_ROWS][MAT_COLS];

  void sendEvent(int r, int c, int val);
  inline void setState(int r, int c, int val) {
    if(matrix[r][c] != val) {
      matrix[r][c] = val;
      sendEvent(r, c, val);
    }
  }
};

#endif
