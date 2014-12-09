/*
 Evaluator.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/

#ifndef INCL_Evaluator_H
#define INCL_Evaluator_H

#include "RunTimeEnvironment.H"
#include "ConsCell.H"
#include "MarkHook.H"
#include "MarkStack.H"
using namespace std;
using namespace BOOM;

// **************************************************
//                class Evaluator
// **************************************************

namespace Lambda {
class Evaluator : public MarkHook
{
  RunTimeEnvironment &env;
  Array1D<LambdaObject*> *
    evaluateParms(Array1D<AstNode*> &expressions);
  MarkStack localMarkStack;
  void purgeMarkStack();
public:
  Evaluator(RunTimeEnvironment &);
  LambdaObject *evaluate(AstNode *,bool tailRecursive=false); //###true);
  LambdaObject *evaluateTopLevel(AstNode *);
  virtual void pushAccessibles(MarkStack &);
};
}

#endif
