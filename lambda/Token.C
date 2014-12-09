/*
 Token.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Token.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


void Lambda::printTokenType(TokenType tokenType,ostream &os)
{
  switch(tokenType)
    {
    case TOK_IDENT:      // any-identifier     list?     array?
      os << "TOK_IDENT";
      break;
    case TOK_STRING_LIT:     // "any string literal"
      os << "TOK_STRING_LIT";
      break;
    case TOK_INT_LIT:        // 123
      os << "TOK_INT_LIT";
      break;
    case TOK_FLOAT_LIT:      // 3.14
      os << "TOK_FLOAT_LIT";
      break;
    case TOK_CHAR_LIT:       // #a     #\n
      os << "TOK_CHAR_LIT";
      break;
    case TOK_SYMBOL:       // 'any-identifier   'list?    'array?
      os << "TOK_SYMBOL";
      break;
    case TOK_TIMES:          // *
      os << "TOK_TIMES";
      break;
    case TOK_DIVIDE:         // /
      os << "TOK_DIVIDE";
      break;
    case TOK_MOD:            // %
      os << "TOK_MOD";
      break;
    case TOK_PLUS:           // +
      os << "TOK_PLUS";
      break;
    case TOK_MINUS:          // -
      os << "TOK_MINUS";
      break;
    case TOK_GREATER:        // >
      os << "TOK_GREATER";
      break;
    case TOK_GREATER_EQUAL:  // >=
      os << "TOK_GREATER_EQUAL";
      break;
    case TOK_LESS:           // <
      os << "TOK_LESS";
      break;
    case TOK_LESS_EQUAL:     // <=
      os << "TOK_LESS_EQUAL";
      break;
    case TOK_EQUAL:          // =
      os << "TOK_EQUAL";
      break;
    case TOK_NOT_EQUAL:      // !=
      os << "TOK_NOT_EQUAL";
      break;
    case TOK_PIPE:           // |
      os << "TOK_PIPE";
      break;
    case TOK_LEFT_BRACKET:   // [
      os << "TOK_LEFT_BRACKET";
      break;
    case TOK_RIGHT_BRACKET:  // ]
      os << "TOK_RIGHT_BRACKET";
      break;
    case TOK_LEFT_PAREN:     // (
      os << "TOK_LEFT_PAREN";
      break;
    case TOK_RIGHT_PAREN:     // )
      os << "TOK_RIGHT_PAREN";
      break;
    case TOK_EOF: // end of file
      os << "TOK_EOF";
      break;
    }
}



ostream &operator<<(ostream &os,const Token &token)
{
  token.printOn(os);
  return os;
}



// **************************************************
//                  Token methods
// **************************************************

Token::Token(const String &lexeme,TokenType tokenType,
	     int lineNum)
  : lexeme(lexeme), tokenType(tokenType), lineNum(lineNum)

{
  // ctor
}



const String &Token::getLexeme() const
{
  return lexeme;
}



TokenType Token::getTokenType() const
{
  return tokenType;
}



int Token::getLineNum() const
{
  return lineNum;
}



void Token::printOn(ostream &os) const
{
  printTokenType(tokenType,os);
  os << " \t" << lexeme;
}




void Token::changeLexeme(const String &newValue)
{
  lexeme=newValue;
}

