/*
 LambdaException.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaException_H
#define INCL_LambdaException_H

#include "BOOM/Exceptions.H"
using namespace std;
using namespace BOOM;

namespace Lambda {
class Token;

// **************************************************
//               class LambdaException
// **************************************************

class LambdaException : public RootException
{
public:
  LambdaException(const String &);
};



// **************************************************
//               class LexicalError
// **************************************************

class LexicalError : public LambdaException
{
public:
  LexicalError(char,int lineNum);
};



// **************************************************
//               class SyntaxError
// **************************************************

class SyntaxError : public LambdaException
{
public:
  SyntaxError(Token *);
};

}

#endif
