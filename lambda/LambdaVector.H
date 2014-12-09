/*
 LambdaVector.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaVector_H
#define INCL_LambdaVector_H
#include "LambdaObject.H"
#include "BOOM/Array1D.H"
using namespace BOOM;

namespace Lambda {
class LambdaVector : public LambdaObject
{
  Array1D<LambdaObject*> array;
public:
  LambdaVector(int size);
  int getSize() const;
  LambdaObject *getElement(int);
  void setElement(int,LambdaObject *);
  virtual void pushAccessibles(MarkStack &);
};
}

#endif
