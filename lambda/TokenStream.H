/*
 TokenStream.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_TokenStream_H
#define INCL_TokenStream_H

#include "Scanner.H"
using namespace BOOM;

namespace Lambda {
class TokenStream
{
  Scanner &scanner;
  Token *pushedBack;
public:
  TokenStream(Scanner *);
  TokenStream(istream &);
  ~TokenStream();
  Token *nextToken();
  void pushBack(Token *);
  TokenType peekTokenType();
};
}

#endif
