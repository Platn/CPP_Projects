// Author: Sean Davis
#ifndef cpuH
#define cpuH

#include "CPURunner.h"

class CPU
{
public:
  CPU();
  void operation(OpCode &opCode, Instruction &instruction, int &buffer);
//    void addFunction();
//    void loadFunction();
//    void storeFunction();
//    void doneFunction();
};
#endif
