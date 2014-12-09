/*
 LambdaChar.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaChar.H"
using namespace Lambda;

LambdaChar::LambdaChar(char value)
  : value(value), LambdaObject(OBTYPE_CHAR)
{
  // ctor
}



char LambdaChar::getValue() const
{
  return value;
}





