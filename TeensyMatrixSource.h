#ifndef TEENSYMATRIXSOURCE_H__
#define TEENSYMATRIXSOURCE_H__

#include "MatrixSource.h"

class TeensyMatrixSource : public MatrixSource
{
public:
  TeensyMatrixSource() : MatrixSource(0) {};
  void start();

  virtual void update();
};

#endif
