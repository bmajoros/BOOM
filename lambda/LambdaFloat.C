/*
 LambdaFloat.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaFloat.H"
using namespace Lambda;

LambdaFloat::LambdaFloat(double f)
  : value(f), LambdaObject(OBTYPE_FLOAT)
{
  // ctor
}



double LambdaFloat::getValue() const
{
  return value;
}





