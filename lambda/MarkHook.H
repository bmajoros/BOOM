/*
 MarkHook.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_MarkHook_H
#define INCL_MarkHook_H
#include "linked.H"
#include "../Vector.H"

namespace Lambda {

class MarkStack;
class Garbage;

/*
 Provides a hook into the 'mark' phase of the 
 'mark & sweep' garbage-collector.  Use this to 
 push any Lambda objects accessible in your C++
 code (but not accessible on the Lambda run-time-
 stack) onto the MarkStack, so they will get
 marked (and thus not collected by the GC).
*/

class MarkHook : public link_node
{
public:

  /*
    Override this method so that it pushes objects onto the
    MarkStack.  Push any object you don't want to get
    garbage-collected, unless that object is already accessible
    through the Lambda run-time stack:
  */

  virtual void pushAccessibles(MarkStack &)=0;
};


  /*
    The following class keeps a list of objects that should never
    be deleted by the GC.
   */
class Immortals : public MarkHook
{
  BOOM::Vector<Garbage*> immortals;
public:
  void add(Garbage *);
  virtual void pushAccessibles(MarkStack &);
};

}

#endif
