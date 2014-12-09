/*
 LambdaInt.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaInt_H
#define INCL_LambdaInt_H

#include "LambdaObject.H"

namespace Lambda {
class LambdaInt : public LambdaObject
{
  int value;
public:
  LambdaInt(int);
  int getValue() const;
  virtual void pushAccessibles(MarkStack &) {}
};
}

#endif
