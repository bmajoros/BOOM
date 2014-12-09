/*
 ObjectPrinter.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_ObjectPrinter_H
#define INCL_ObjectPrinter_H
#include <iostream>
#include "LambdaInt.H"
#include "LambdaFloat.H"
#include "LambdaChar.H"
#include "LambdaString.H"
#include "Closure.H"
#include "ForeignFunctionObject.H"
#include "ConsCell.H"
#include "Symbol.H"
#include "LambdaVector.H"
using namespace std;
using namespace BOOM;

namespace Lambda {
class ObjectPrinter
{
  RunTimeEnvironment &env;

  void printInt(LambdaInt *,ostream &);
  void printChar(LambdaChar *,ostream &);
  void printFloat(LambdaFloat *,ostream &);
  void printString(LambdaString *,ostream &);
  void printClosure(Closure *,ostream &);
  void printForeignFunc(ForeignFunctionObject *,ostream &);
  void printList(ConsCell *,ostream &);
  void printProperList(ConsCell *,ostream &);
  void printImproperList(ConsCell *,ostream &);
  void printForeignType(LambdaObject *,ostream &);
  void printSymbol(Symbol *,ostream &);
  void printVector(LambdaVector *,ostream &);
public:
  ObjectPrinter(RunTimeEnvironment &);
  void print(LambdaObject *,ostream &);
};
}

#endif
