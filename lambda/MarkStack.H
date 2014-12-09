/*
 MarkStack.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_MarkStack_H
#define INCL_MarkStack_H
#include "BOOM/Stack.H"
using namespace BOOM;

namespace Lambda {
class Garbage;

// **************************************************
//                class MarkStack
// **************************************************

/*
 Used by the GarbageCollector during the mark phase.
 The GC asks all objects it encounters to push all
 objects accessible through them onto this stack.
 The GC then pops things off the stack, marks
 them, and then lets them push their accessibles
 as well.  Thus, a depth-first-search is effected.
 This minimizes the GC's use of the (C++) 
 run-time-stack.
*/

class MarkStack
{
  Stack<Garbage*> stack;
public:
  MarkStack();
  void push(Garbage *);
  Garbage *pop();
  void clear();
};
}

#endif
