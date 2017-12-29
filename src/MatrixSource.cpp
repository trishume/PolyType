#include "MatrixSource.h"
#include <Arduino.h>

MatrixSource::MatrixSource(int offset) : out(0), colOffset(offset) {
  for(int i=0; i<MAT_ROWS; i++){
    for(int j=0; j<MAT_COLS; j++) {
      matrix[i][j] = 0;
      lastEvent[i][j] = 0;
    }
  }
  scanStart = 0;
}

void MatrixSource::startingScan() {
  scanStart = micros();
}

void MatrixSource::sendEvent(int r, int c, int val) {
  if(out == 0) return;
  KeyEventType type = val ? KeyDown : KeyUp;
  KeyMatrixEvent event(r, c + colOffset, type);
  out->push(event);
}
