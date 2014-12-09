/*
 LambdaException.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaException.H"
#include <sstream>
#include "Token.H"
#include "BOOM/String.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

// **************************************************
//              LambdaException methods
// **************************************************

LambdaException::LambdaException(const String &so)
  : RootException(so)
{
  // ctor
}



// **************************************************
//               LexicalError methods
// **************************************************

LexicalError::LexicalError(char c,int lineNum)
  : LambdaException("")
{
  // ctor

  ostringstream str;
  str << "Lexical error at '" << c << "' on line " << lineNum;
  setMessage(str.str());
  //str.rdbuf()->freeze(0);
}



// **************************************************
//               SyntaxError methods
// **************************************************

SyntaxError::SyntaxError(Token *token)
  : LambdaException("")
{
  // ctor

  ostringstream str;
  str << "Syntax error on line " << token->getLineNum()
      << " near \"" << token->getLexeme() << "\"";
  setMessage(str.str());
  //str.rdbuf()->freeze(0);

  delete token;
}





