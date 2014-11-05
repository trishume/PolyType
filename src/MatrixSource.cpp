#include "MatrixSource.h"

MatrixSource::MatrixSource(int offset) : colOffset(offset), out(0) {
  for(int i=0; i<MAT_ROWS; i++){
    for(int j=0; j<MAT_COLS; j++) {
      matrix[i][j] = 0;
    }
  }
}

void MatrixSource::sendEvent(int r, int c, int val) {
  if(out == 0) return;
  KeyEventType type = val ? KeyDown : KeyUp;
  KeyMatrixEvent event(r, c + colOffset, type);
  out->push(event);
}


