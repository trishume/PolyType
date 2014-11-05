#ifndef __PIPE_H__
#define __PIPE_H__

template<typename T>
class Pipe
{
public:
  virtual ~Pipe() {};

  virtual void push(const T &packet) = 0;
};

#endif
