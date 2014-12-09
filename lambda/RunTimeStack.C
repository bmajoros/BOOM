/*
 RunTimeStack.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "RunTimeStack.H"
#include "LambdaException.H"
using namespace BOOM;
using namespace Lambda;


LambdaObject *RunTimeStack::retrieve(LexicalAddress la)
{

  // This method retrieves an object at the specified
  // lexical address, (: d p), where d=lexical depth
  // and p=lexical position.  d is the depth in the
  // stack (in # activation records) and p is position
  // within an AR (in # objects).

  // Start at the top of the stack
  ActivationRecord *ar=peekTop();

  // Descend to the specified lexical depth by following
  // that many static chains
  for(int i=0 ; i<la.getDepth() ; ++i)
    {
      ar=ar->getStaticChain();

      // ### Get rid of this...
      /*
      if(ar==NULL)
	throw LambdaException(
	  "Bottom of stack reached in RunTimeStack.retrieve()");
      */
    }

  // Get the p-th object at that depth, where p is the
  // given lexical position
  LambdaObject *obj=ar->getEntry(la.getPosition());
  return obj;
}



void RunTimeStack::set(LexicalAddress la,LambdaObject *object)
{
  // Start at the top of the stack
  ActivationRecord *ar=peekTop();

  // Descend to the specified lexical depth by following
  // that many static chains
  for(int i=0 ; i<la.getDepth() ; ++i)
    {
      ar=ar->getStaticChain();
      if(!ar) throw LambdaException("Invalid lexical depth");
    }

  /*---- already checked in ActivationRecord::setEntry() ----
  // Verify that lexical position is valid
  if(la.getPosition()<0 || 
     la.getPosition()>ar->getNumEntries())
    throw LambdaException("Invalid lexical position");
  */

  // Store the object at the specified lexical position
  ar->setEntry(la.getPosition(),object);
}



ActivationRecord *RunTimeStack::getGlobalAR()
{
  // Returns a pointer to the AR on the bottom of the stack,
  // which holds global objects
  
  return (ActivationRecord *) stack.bottom();
}



void RunTimeStack::pushAR(int AR_size,
			  ActivationRecord *staticChain)
{
  // Create a new activation record
  ActivationRecord *ar=new ActivationRecord(AR_size,staticChain);
  
  // push onto stack
  stack.push(ar);
}



void RunTimeStack::pushAR(ActivationRecord *ar)
{
  stack.push(ar);
}



void RunTimeStack::popAR(GarbageCollector &gc)
{
  // pop the top AR off the stack
  ActivationRecord *ar=(ActivationRecord *) stack.pop();
}



ActivationRecord *RunTimeStack::peekTop()
{
  return (ActivationRecord*) stack.top();
}



bool RunTimeStack::isEmpty()
{
  return stack.isEmpty();
}



void RunTimeStack::initIterator()
{
  iterator=0;
}



bool RunTimeStack::canIterate()
{
  return iterator<stack.size();
}



ActivationRecord *RunTimeStack::iterate()
{
  return (ActivationRecord *) stack.asVector()[iterator++];
}



bool RunTimeStack::topARisGlobal()
{
  return stack.size()==1;
}



