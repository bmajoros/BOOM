/*
 FunctionObject.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "FunctionObject.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;



FunctionObject::FunctionObject(LambdaObjectType t,int arity)
  : LambdaObject(t), arity(arity)
{
  // ctor
}



int FunctionObject::getArity() const
{
  return arity;
}




