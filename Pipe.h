#ifndef __PIPE_H__
#define __PIPE_H__

template<typename T>
class Pipe
{
public:
  virtual ~Pipe() {};

  virtual void push(T &packet) = 0;
};

#endif
