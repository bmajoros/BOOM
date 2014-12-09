/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_GC_H
#define INCL_GC_H

#include "Garbage.H"
#include "MarkStack.H"
#include "BOOM/Time.H"
using namespace std;
using namespace BOOM;

namespace Lambda {
class RunTimeStack;

//const int GC_THRESHOLD=3000000;//100000;
extern int GC_THRESHOLD;

// **************************************************
//             class GarbageCollector
// **************************************************

// (not to be confused with a NectarCollector, which
// is another name for a hummingbird)

class GarbageCollector
{
  MarkStack &markStack;
  linked_list &markHooks;
  linked_list &list; // list of Garbage
  RunTimeStack *runTimeStack;
  int numObjects;//total # objects in list
  int inUse;// approx. # objects still accessible
  int threshold;// run GC when numObjects>inUse+threshold
  bool silent;
  Time &timer;

  void markPhase();
  void sweepPhase();
  void processMarkStack();
public:
  GarbageCollector(int threshold=GC_THRESHOLD);
  virtual ~GarbageCollector();
  void deleteEverything();
  void registerObject(Garbage *);
  void unregisterObject(Garbage *);
  void collect(RunTimeStack &);
  bool collectIfNecessary(RunTimeStack &);
  inline bool shouldCollect() { return numObjects>inUse+threshold; }
  int numLiveObjects() const;
  void registerMarkHook(MarkHook *);
  void setThreshold(int);
  void setSilence(bool);
  bool isSilent() {return silent;}
};

}

#endif
