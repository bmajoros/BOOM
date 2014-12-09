/*
 ForeignFunctionObject.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_ForeignFunctionObject_H
#define INCL_ForeignFunctionObject_H

#include "FunctionObject.H"
using namespace std;
using namespace BOOM;

namespace Lambda {

class RunTimeEnvironment;

// **************************************************
//                enum ContinuationType
// **************************************************

// What to do with the value returned from a ForeignFunction

enum ContinuationType
{
  CONT_TAIL_REC_CALL, // Make a tail-recursive call to return-value
  CONT_SIMPLE_RETURN  // Simply return the return-value
};



// **************************************************
//                typedef ForeignFunction
// **************************************************

/*
 A ForeignFunction is a C++ function 
 taking an ActivationRecord (for retrieving 
 parameters) and ContinationType (for 
 specifying what to do with the return 
 value) and returning a LambdaObject.

 You can set arity to -1 to indicate variable
 arity; the activation record will be an 
 ExpandableAR, so you can query it at runtime
 to see how many arguments are in the AR.
 Otherwise, if more arguments are provided 
 than declared, the extras will come as a
 cons list in the last parameter (unless you
 declare arity of 0, in which case you get
 nothing!).

 If your ForeignFunction needs to make a 
 tail-recursive call, don't do it!  Instead, 
 package the call as a FunctionObject,
 return that object, and set ContinationType 
 to CONT_TAIL_REC_CALL.  The Lambda 
 interpreter will make the call for you 
 after returning.  Otherwise, simply return 
 your return value and set ContinuationType
 to CONT_SIMPLE_RETURN.
*/

typedef LambdaObject *(*ForeignFunction)(RunTimeEnvironment &,
					ContinuationType &);


const int VARIADIC=-1;


// **************************************************
//                class ForeignFunctionObject
// **************************************************

class ForeignFunctionObject : public FunctionObject
{
  ForeignFunction function;
public:
  ForeignFunctionObject(ForeignFunction,int arity=VARIADIC);
  ForeignFunction getCPlusPlusFunction();
  virtual void pushAccessibles(MarkStack &) {}
};

}

#endif
