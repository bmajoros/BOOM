/****************************************************************
 GenericToken.H
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GenericToken_H
#define INCL_GenericToken_H
#include <iostream>
#include "String.H"
using namespace std;

namespace BOOM {

// **************************************************
//                 enum GenericTokenType
// **************************************************

enum GenericTokenType
{
  TOK_IDENT,          // any_identifier123
  TOK_STRING_LIT,     // "any string literal"
  TOK_INT_LIT,        // 123
  TOK_FLOAT_LIT,      // 3.14
  TOK_PERIOD,         // .
  TOK_CHAR_LIT,       // 'a'  '\n'
  TOK_TIMES,          // *
  TOK_DOUBLE_STAR,    // **
  TOK_TIMES_EQUAL,    // *=
  TOK_DIVIDE,         // /
  TOK_DIVIDE_EQUAL,   // /=
  TOK_MOD,            // %
  TOK_PLUS,           // +
  TOK_PLUS_EQUAL,     // +=
  TOK_INCREMENT,      // ++
  TOK_MINUS,          // -
  TOK_DECREMENT,      // --
  TOK_MINUS_EQUAL,    // -=
  TOK_ARROW,          // ->
  TOK_GREATER,        // >
  TOK_GREATER_EQUAL,  // >=
  TOK_SHIFT_RIGHT,    // >>
  TOK_LESS,           // <
  TOK_LESS_EQUAL,     // <=
  TOK_SHIFT_LEFT,     // <<
  TOK_EQUAL,          // =
  TOK_IS_EQUAL,       // ==
  TOK_DOUBLE_ARROW,   // =>
  TOK_NOT,            // !
  TOK_NOT_EQUAL,      // !=
  TOK_PIPE,           // |
  TOK_LEFT_BRACKET,   // [
  TOK_RIGHT_BRACKET,  // ]
  TOK_LEFT_PAREN,     // (
  TOK_RIGHT_PAREN,    // )
  TOK_LEFT_BRACE,     // {
  TOK_RIGHT_BRACE,    // }
  TOK_CARET,          // ^
  TOK_HASH,           // #
  TOK_TILDE,          // ~
  TOK_TILDE_EQUAL,    // ~=
  TOK_AT,             // @
  TOK_DOLLAR,         // $
  TOK_AND,            // &
  TOK_DOUBLE_AND,     // &&
  TOK_AND_EQUAL,      // &=
  TOK_COLON,          // :
  TOK_SEMICOLON,      // ;
  TOK_COMMA,          // ,
  TOK_QUESTION,       // ?
  TOK_EOF,            // end of file
  TOK_KEYWORD         // (register these with the scanner)
};

ostream &operator<<(ostream &,GenericTokenType);
void printGenericTokenType(GenericTokenType,ostream &);
const char *toString(GenericTokenType);



// **************************************************
//                  class GenericToken
// **************************************************

class GenericToken
{
  String lexeme;
  GenericTokenType tokenType;
  int lineNum;
public:
  GenericToken(const String &,GenericTokenType,int lineNum);
  const String &getLexeme() const;
  GenericTokenType getTokenType() const;
  int tokenTypeAsInt() const; // so you can use any enum w/keywords defined
  int getLineNum() const;
  void printOn(ostream &) const;
  void changeLexeme(const String &);
  bool isEof();
};

ostream &operator<<(ostream &,const GenericToken &);

}

#endif

