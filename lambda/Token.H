/*
 Token.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_Token_H
#define INCL_Token_H

#include "BOOM/String.H"
#include <iostream>
using namespace std;
using namespace BOOM;
namespace Lambda {


// **************************************************
//                 enum TokenType
// **************************************************

enum TokenType
{
  TOK_IDENT,          // any-identifier     list?     array?
  TOK_STRING_LIT,     // "any string literal"
  TOK_INT_LIT,        // 123
  TOK_FLOAT_LIT,      // 3.14
  TOK_CHAR_LIT,       // #a     #\n
  TOK_SYMBOL,         // 'any-identifier   'list?    'array?
  TOK_TIMES,          // *
  TOK_DIVIDE,         // /
  TOK_MOD,            // %
  TOK_PLUS,           // +
  TOK_MINUS,          // -
  TOK_GREATER,        // >
  TOK_GREATER_EQUAL,  // >=
  TOK_LESS,           // <
  TOK_LESS_EQUAL,     // <=
  TOK_EQUAL,          // =
  TOK_NOT_EQUAL,      // !=
  TOK_PIPE,           // |
  TOK_LEFT_BRACKET,   // [
  TOK_RIGHT_BRACKET,  // ]
  TOK_LEFT_PAREN,     // (
  TOK_RIGHT_PAREN,    // )
  TOK_EOF             // end of file
};


void printTokenType(TokenType,ostream &);


// **************************************************
//                  class Token
// **************************************************

class Token
{
  String lexeme;
  TokenType tokenType;
  int lineNum;
public:
  Token(const String &,TokenType,int lineNum);
  const String &getLexeme() const;
  TokenType getTokenType() const;
  int getLineNum() const;
  void printOn(ostream &) const;
  void changeLexeme(const String &);
};

ostream &operator<<(ostream &,const Token &);

}

#endif
