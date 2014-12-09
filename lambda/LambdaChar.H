/*
 LambdaChar.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaChar_H
#define INCL_LambdaChar_H

#include "LambdaObject.H"

namespace Lambda {
class LambdaChar : public LambdaObject
{
  char value;
public:
  LambdaChar(char);
  char getValue() const;
  virtual void pushAccessibles(MarkStack &) {}
};
}

#endif
