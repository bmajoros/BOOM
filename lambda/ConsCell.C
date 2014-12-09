/*
 ConsCell.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "ConsCell.H"
#include "MarkStack.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


ConsCell::ConsCell(LambdaObject *car,LambdaObject *cdr)
  : car(car), cdr(cdr), LambdaObject(OBTYPE_CONS_CELL)
{
  // ctor
}



LambdaObject *ConsCell::getCar() const
{
  return car;
}



LambdaObject *ConsCell::getCdr() const
{
  return cdr;
}



void ConsCell::pushAccessibles(MarkStack &markStack)
{
  markStack.push(cdr);//deeper = process later
  markStack.push(car);//shallower = process sooner
}




