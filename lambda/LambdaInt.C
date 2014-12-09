/*
 LambdaInt.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaInt.H"
using namespace Lambda;


LambdaInt::LambdaInt(int value)
  : value(value), LambdaObject(OBTYPE_INT)
{
  // ctor
}



int LambdaInt::getValue() const
{
  return value;
}







