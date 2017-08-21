// Author: Sean Davis
#include "cpu.h"

CPU::CPU()
{
} // CPU()


void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer)
{
  switch (opCode)
  {
    case ADD  :
          instruction.addr3 = instruction.addr1 + instruction.addr2;
          // *addr3 = *addr1 + *addr2
          break;
    case LOAD :  // buffer contains int requested from RAM
          break;
    case STORE:  // Sent by RAM after a STORE
          break;
    case DONE : ; //  All ADDs in file have been sent.  Time to STORE dirty cache.
          break;
  } // switch
} // operation()

