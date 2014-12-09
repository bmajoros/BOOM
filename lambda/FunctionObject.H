/*
 FunctionObject.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/

#ifndef INCL_FunctionObject_H
#define INCL_FunctionObject_H

#include "LambdaObject.H"
using namespace std;
using namespace BOOM;


/*
 An object which is a function.  It will either
 be a Lambda function (represented by class Closure)
 or a C++ function (represented by class
 ForeignFunctionObject).
*/

// Abstract class

namespace Lambda {
class FunctionObject : public LambdaObject
{
  int arity;
protected:
  FunctionObject(LambdaObjectType,int arity); // for subclasses
public:
  int getArity() const;
};
}

#endif
