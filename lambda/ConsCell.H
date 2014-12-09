/*
 ConsCell.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_ConsCell_H
#define INCL_ConsCell_H

#include "LambdaObject.H"
using namespace std;
using namespace BOOM;

namespace Lambda {
class MarkStack;

class ConsCell : public LambdaObject
{
  LambdaObject *car, *cdr;
public:
  ConsCell(LambdaObject *car,LambdaObject *cdr);
  LambdaObject *getCar() const;
  LambdaObject *getCdr() const;
  virtual void pushAccessibles(MarkStack &);
};
}

#endif
