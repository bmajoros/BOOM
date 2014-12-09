/*
 Closure.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Closure.H"
#include "AR.H"
#include "AST.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

Closure::Closure(ActivationRecord *staticChain,
		 AstLambda *function)
  : function(function), staticChain(staticChain),
    FunctionObject(OBTYPE_CLOSURE,function->getArity())
{
  // ctor
}



ActivationRecord *Closure::getStaticChain() const
{
  return staticChain;
}



AstLambda *Closure::getFunction() const
{
  return function;
}



void Closure::pushAccessibles(MarkStack &markStack)
{
  markStack.push(staticChain);
  markStack.push(function);
}



void Closure::setStaticChain(ActivationRecord *c)
{
  staticChain=c;
}



Closure *Closure::deepCopy() const
{
  return new Closure(staticChain,
		     static_cast<AstLambda*>(function->deepCopy()));
}


