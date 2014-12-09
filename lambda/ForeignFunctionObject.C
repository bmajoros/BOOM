/*
 ForeignFunctionObject.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "ForeignFunctionObject.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


ForeignFunctionObject::ForeignFunctionObject(
  ForeignFunction function,int arity)
  : function(function), 
    FunctionObject(OBTYPE_FOREIGN_FUNCTION,arity)
{
  // ctor
}



ForeignFunction ForeignFunctionObject::getCPlusPlusFunction()
{
  return function;
}





