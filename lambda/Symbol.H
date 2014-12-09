/*
 Symbol.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_Lambda_Symbol_H
#define INCL_Lambda_Symbol_H
#include "BOOM/String.H"
#include "LambdaObject.H"
using namespace std;
using namespace BOOM;

namespace Lambda {
class Symbol : public LambdaObject
{
  const String &lexeme;
public:
  Symbol(const String &lexeme);
  const String &getLexeme();
  virtual void pushAccessibles(MarkStack &) {}
};
}

#endif
