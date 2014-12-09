/*
 Closure.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_Closure_H
#define INCL_Closure_H

#include "FunctionObject.H"

using namespace std;
using namespace BOOM;

namespace Lambda {

class AstLambda;
class ActivationRecord;


// Represents a (Lambda) function and its lexical context.

class Closure : public FunctionObject
{
  ActivationRecord *staticChain;
  AstLambda *function;
public:
  Closure(ActivationRecord *staticChain,AstLambda *);
  ActivationRecord *getStaticChain() const;
  void setStaticChain(ActivationRecord *);
  AstLambda *getFunction() const;
  virtual void pushAccessibles(MarkStack &);
  Closure *deepCopy() const;
};

}

#endif
