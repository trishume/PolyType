#ifndef MCPMATRIXSOURCE_H__
#define MCPMATRIXSOURCE_H__

#include "MatrixSource.h"

class MCPMatrixSource : public MatrixSource
{
public:
  MCPMatrixSource() : MatrixSource(MAT_COLS) {};
  void start();

  virtual void update();
};

#endif
