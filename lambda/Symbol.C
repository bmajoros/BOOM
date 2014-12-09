/*
 Symbol.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Symbol.H"
using namespace Lambda;

Symbol::Symbol(const String &lexeme)
  : lexeme(lexeme), LambdaObject(OBTYPE_SYMBOL)
{
  // ctor
}


const String &Symbol::getLexeme()
{
  return lexeme;
}



