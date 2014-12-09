/*
 RunTimeStack.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_RunTimeStack_H
#define INCL_RunTimeStack_H
#include "LambdaObject.H"
#include "AR.H"
#include "GC.H"
#include "LexicalAddress.H"
using namespace std;
using namespace BOOM;
namespace Lambda {

// ****************************************
//	      class AR_stack
// ****************************************
typedef ActivationRecord * ARptr;
typedef Stack<ARptr> AR_stack; 



// ****************************************
//	     class RunTimeStack
// ****************************************

/*
 The RunTimeStack is a stack of activation
 records, which hold local variables, parameters,
 and temporaries for the current method
 invocation.
*/

class RunTimeStack 
{
  AR_stack stack;
  ActivationRecord &getARatDepth(int depth);
  int iterator;
public:
  RunTimeStack() {}

  // For normal use:
  void pushAR(int AR_size,ActivationRecord *staticChain);
  void pushAR(ActivationRecord *);
  void popAR(GarbageCollector &GC);
  ActivationRecord *peekTop();
  ActivationRecord *getGlobalAR();
  bool isEmpty();
  LambdaObject *retrieve(LexicalAddress);
  void set(LexicalAddress,LambdaObject *);
  bool topARisGlobal();

  // For garbage collection:
  void initIterator();
  bool canIterate();
  ActivationRecord *iterate();
};

}

#endif
