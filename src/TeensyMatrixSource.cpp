#include "TeensyMatrixSource.h"
#include "Arduino.h"

static const byte rowPins[MAT_ROWS] = {0,1,2,3,22,21};
static const byte colPins[MAT_COLS] = {4,5,6,7,8,9,10};

void TeensyMatrixSource::start() {
  for(int i=0;i<MAT_ROWS;i++)
    pinMode(rowPins[i],OUTPUT);
  for(int i=0;i<MAT_COLS;i++)
    pinMode(colPins[i],INPUT_PULLUP);
}

void TeensyMatrixSource::update() {
  pinMode(10,INPUT_PULLUP);
  for(int r=0; r<MAT_ROWS; r++) {
    for(int i=0;i<MAT_ROWS;i++) {
      pinMode(rowPins[i],INPUT);
    }
    pinMode(rowPins[r],OUTPUT);
    digitalWrite(rowPins[r],LOW);
    delay(2);
    for(int c=0; c<MAT_COLS; c++) {
      int val = !digitalRead(colPins[c]);
      int matC = c;
      setState(r, c, val);
    }
  }
}
