/*
 LambdaString.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaString.H"
using namespace Lambda;

LambdaString::LambdaString(const String &value)
  : value(value), LambdaObject(OBTYPE_STRING)
{
  // ctor
}



const String &LambdaString::getValue() const
{
  return value;
}





