#ifndef MATRIXSOURCE_H__
#define MATRIXSOURCE_H__

#include "PacketTypes.h"
#include "Pipe.h"
#include <Arduino.h>

#define MAT_ROWS 6
#define MAT_COLS 7

// The debounce delay before re-polling a key in microseconds
#define DEBOUNCE_TIME 100000

class MatrixSource
{
public:
  MatrixSource(int offset);
  Pipe<KeyMatrixEvent> *out;
  int colOffset;

  virtual void update() = 0;
protected:
  int matrix[MAT_ROWS][MAT_COLS];
  unsigned long lastEvent[MAT_ROWS][MAT_COLS];
  unsigned long scanStart;

  void sendEvent(int r, int c, int val);
  void startingScan();
  inline void setState(int r, int c, int val) {
    if(matrix[r][c] != val) {
      unsigned long last = lastEvent[r][c];
      // handle overflow wrapping, which leads to
      if(!(scanStart < last || scanStart >= lastEvent[r][c] + DEBOUNCE_TIME)) {
        // Serial.println("debounce");
        return;
      }
      matrix[r][c] = val;
      lastEvent[r][c] = scanStart;
      sendEvent(r, c, val);
    }
  }
};

#endif
