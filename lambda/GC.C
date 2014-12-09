/*
 GC.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "GC.H"
#include "RunTimeStack.H"
#include <iostream>
using namespace std;
using namespace BOOM;
using namespace Lambda;


int Lambda::GC_THRESHOLD=3000000;//100000;


GarbageCollector::GarbageCollector(int threshold)
  : runTimeStack(NULL), numObjects(0),
    threshold(threshold), inUse(0), silent(true),
    markStack(*new MarkStack),
    markHooks(*new linked_list),
    list(*new linked_list),
    timer(*new Time)
{
  // ctor
}



GarbageCollector::~GarbageCollector()
{
  delete &list;
  markHooks.LetGoOfAll();
  delete &markHooks;
  delete &markStack;
}



bool GarbageCollector::collectIfNecessary(RunTimeStack &runTimeStack)
{
  if(shouldCollect())
    {
      collect(runTimeStack);
      return true;
    }
  return false;
}




void GarbageCollector::registerObject(Garbage *g)
{
  list.list_insert(g);
  ++numObjects;
}



void GarbageCollector::collect(RunTimeStack &s)
{
  runTimeStack=&s;

  // Perform "mark & sweep garbage collection"

  if(!silent) {
    cerr<<"Collecting garbage... ";
    timer.startCounting();
  }

  // Mark all objects accessible through the run-time stack
  markPhase();

  // Delete any object which is not marked
  sweepPhase();

  //inUse=numObjects;
  if(numObjects>inUse) inUse=numObjects;

  if(!silent) {
    timer.stopCounting();
    cerr<<timer.elapsedTime()<<endl;
  }	
}



void GarbageCollector::processMarkStack()
{
  Garbage *g;
  while(g=markStack.pop())
    {
      if(g->isMarked())
	continue;
      g->mark();
      g->pushAccessibles(markStack);
    }
}



void GarbageCollector::markPhase()
{
  // Let the user specially mark objects not visible from the 
  // Lambda run-time stack
  markHooks.reset_seq();
  MarkHook *markHook;
  while(markHook=static_cast<MarkHook*>(markHooks.sequential()))
    {
      markHook->pushAccessibles(markStack);
      processMarkStack();
    }

  // Mark all objects accessible through the run-time stack
  runTimeStack->initIterator();
  while(runTimeStack->canIterate())
    {
      ActivationRecord *ar=runTimeStack->iterate();
      ar->mark();
      ar->pushAccessibles(markStack);
      processMarkStack();
    }
}



void GarbageCollector::deleteEverything()
{
  list.reset_seq();
  Garbage *g;
  while(g=static_cast<Garbage*>(list.sequential())) {
    list.del_seq();
  }
  numObjects=0;
}



void GarbageCollector::sweepPhase()
{
  list.reset_seq();
  Garbage *g;
  while(g=static_cast<Garbage*>(list.sequential()))
    {
      if(g->isMarked())
	g->unMark();
      else
	{
	  list.del_seq();
	  --numObjects;
	}
    }
}



int GarbageCollector::numLiveObjects() const
{
  return numObjects;
}



void GarbageCollector::unregisterObject(Garbage *g)
{
  // This is very expensive (linear in the amount 
  // of garbage currently registered with the GC),
  // unless g has only recently been registered.
  // In that case, g will be fairly early in the
  // list, and it can be found and removed rather
  // quickly.  Thus, if you call the evaluator
  // to get an object & then immediately unregister
  // it, this should be pretty efficient as long
  // as the amount of computation that went into
  // computing the object was minimal.

  list.LetGoOf(g);
}



void GarbageCollector::registerMarkHook(MarkHook *hook)
{
  // NOTE: hook must *not* be registered with the GC as garbage!!!!
  markHooks.list_insert(hook);
}



void GarbageCollector::setThreshold(int t)
{
  //GC_THRESHOLD=t;
  threshold=t;
}



void GarbageCollector::setSilence(bool s)
{
  silent=s;
}




