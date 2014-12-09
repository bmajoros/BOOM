/*
 LambdaString.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaString_H
#define INCL_LambdaString_H

#include "BOOM/String.H"
#include "LambdaObject.H"
using namespace BOOM;

namespace Lambda {
class LambdaString : public LambdaObject
{
  String value;
public:
  LambdaString(const String &);
  const String &getValue() const;
  virtual void pushAccessibles(MarkStack &) {}
};
}

#endif
