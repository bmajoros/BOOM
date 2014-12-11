/****************************************************************
 GenericToken.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GenericToken.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;


ostream &BOOM::operator<<(ostream &os,GenericTokenType t)
{
  printGenericTokenType(t,os);
}



void BOOM::printGenericTokenType(GenericTokenType tokenType,ostream &os)
{
  switch(tokenType) {
  case TOK_IDENT: os << "TOK_IDENT"; break;
  case TOK_STRING_LIT: os << "TOK_STRING_LIT"; break;
  case TOK_INT_LIT: os << "TOK_INT_LIT"; break;
  case TOK_FLOAT_LIT: os << "TOK_FLOAT_LIT"; break;
  case TOK_PERIOD: os << "TOK_PERIOD"; break;
  case TOK_CHAR_LIT: os << "TOK_CHAR_LIT"; break;
  case TOK_TIMES: os << "TOK_TIMES"; break;
  case TOK_TIMES_EQUAL: os << "TOK_TIMES_EQUAL"; break;
  case TOK_DIVIDE: os << "TOK_DIVIDE"; break;
  case TOK_DIVIDE_EQUAL: os << "TOK_DIVIDE_EQUAL"; break;
  case TOK_MOD: os << "TOK_MOD"; break;
  case TOK_PLUS: os << "TOK_PLUS"; break;
  case TOK_PLUS_EQUAL: os << "TOK_PLUS_EQUAL"; break;
  case TOK_INCREMENT: os << "TOK_INCREMENT"; break;
  case TOK_MINUS: os << "TOK_MINUS"; break;
  case TOK_DECREMENT: os << "TOK_DECREMENT"; break;
  case TOK_MINUS_EQUAL: os << "TOK_MINUS_EQUAL"; break;
  case TOK_ARROW: os << "TOK_ARROW"; break;
  case TOK_GREATER: os << "TOK_GREATER"; break;
  case TOK_GREATER_EQUAL: os << "TOK_GREATER_EQUAL"; break;
  case TOK_SHIFT_RIGHT: os << "TOK_SHIFT_RIGHT"; break;
  case TOK_SHIFT_LEFT: os << "TOK_SHIFT_LEFT"; break;
  case TOK_LESS: os << "TOK_LESS"; break;
  case TOK_LESS_EQUAL: os << "TOK_LESS_EQUAL"; break;
  case TOK_EQUAL: os << "TOK_EQUAL"; break;
  case TOK_IS_EQUAL: os << "TOK_IS_EQUAL"; break;
  case TOK_DOUBLE_ARROW: os << "TOK_DOUBLE_ARROW"; break;
  case TOK_NOT: os << "TOK_NOT"; break;
  case TOK_NOT_EQUAL: os << "TOK_NOT_EQUAL"; break;
  case TOK_PIPE: os << "TOK_PIPE"; break;
  case TOK_LEFT_BRACKET: os << "TOK_LEFT_BRACKET"; break;
  case TOK_RIGHT_BRACKET: os << "TOK_RIGHT_BRACKET"; break;
  case TOK_LEFT_PAREN: os << "TOK_LEFT_PAREN"; break;
  case TOK_RIGHT_PAREN: os << "TOK_RIGHT_PAREN"; break;
  case TOK_LEFT_BRACE: os << "TOK_LEFT_BRACE"; break;
  case TOK_RIGHT_BRACE: os << "TOK_RIGHT_BRACE"; break;
  case TOK_CARET: os << "TOK_CARET"; break;
  case TOK_HASH: os << "TOK_HASH"; break;
  case TOK_TILDE: os << "TOK_TILDE"; break;
  case TOK_TILDE_EQUAL: os << "TOK_TILDE_EQUAL"; break;
  case TOK_AT: os << "TOK_AT"; break;
  case TOK_DOLLAR: os << "TOK_DOLLAR"; break;
  case TOK_AND: os << "TOK_AND"; break;
  case TOK_DOUBLE_AND: os << "TOK_DOUBLE_AND"; break;
  case TOK_AND_EQUAL: os << "TOK_AND_EQUAL"; break;
  case TOK_DOUBLE_STAR: os << "TOK_DOUBLE_STAR"; break;
  case TOK_COLON: os << "TOK_COLON"; break;
  case TOK_SEMICOLON: os << "TOK_SEMICOLON"; break;
  case TOK_COMMA: os << "TOK_COMMA"; break;
  case TOK_QUESTION: os << "TOK_QUESTION"; break;
  case TOK_EOF: os << "TOK_EOF"; break;
  case TOK_KEYWORD: os << "TOK_KEYWORD"; break;
  default: os << "(unknown)"; break; // could be a registered keyword
  }
}



const char *BOOM::toString(GenericTokenType t)
{
  switch(t) {
  case TOK_IDENT: return "identifier";
  case TOK_STRING_LIT: return "string literal";
  case TOK_INT_LIT: return "integer";
  case TOK_FLOAT_LIT: return "float";
  case TOK_PERIOD: return ".";
  case TOK_CHAR_LIT: return "character";
  case TOK_TIMES: return "*";
  case TOK_DOUBLE_STAR: return "**";
  case TOK_TIMES_EQUAL: return "*=";
  case TOK_DIVIDE: return "/";
  case TOK_DIVIDE_EQUAL: return "/=";
  case TOK_MOD: return "%";
  case TOK_PLUS: return "+";
  case TOK_PLUS_EQUAL: return "+=";
  case TOK_INCREMENT: return "++";
  case TOK_MINUS: return "-";
  case TOK_DECREMENT: return "--";
  case TOK_MINUS_EQUAL: return "-=";
  case TOK_ARROW: return "->";
  case TOK_GREATER: return ">";
  case TOK_GREATER_EQUAL: return ">=";
  case TOK_SHIFT_RIGHT: return ">>";
  case TOK_LESS: return "<";
  case TOK_LESS_EQUAL: return "<=";
  case TOK_SHIFT_LEFT: return "<<";
  case TOK_EQUAL: return "=";
  case TOK_IS_EQUAL: return "==";
  case TOK_DOUBLE_ARROW: return "=>";
  case TOK_NOT: return "!";
  case TOK_NOT_EQUAL: return "!=";
  case TOK_PIPE: return "|";
  case TOK_LEFT_BRACKET: return "[";
  case TOK_RIGHT_BRACKET: return "]";
  case TOK_LEFT_PAREN:     return "(";
  case TOK_RIGHT_PAREN:    return ")";
  case TOK_LEFT_BRACE:     return "{";
  case TOK_RIGHT_BRACE:    return "}";
  case TOK_CARET:          return "^";
  case TOK_HASH:           return "#";
  case TOK_TILDE:          return "~";
  case TOK_TILDE_EQUAL:    return "~=";
  case TOK_AT:             return "@";
  case TOK_DOLLAR:         return "$";
  case TOK_AND:            return "&";
  case TOK_DOUBLE_AND:     return "&&";
  case TOK_AND_EQUAL:      return "&=";
  case TOK_COLON:          return ":";
  case TOK_SEMICOLON:      return ";";
  case TOK_COMMA:          return ",";
  case TOK_QUESTION:       return "?";
  case TOK_EOF:            return "end of file";
  case TOK_KEYWORD:        return "keyword";
  }
}



ostream &BOOM::operator<<(ostream &os,const GenericToken &token)
{
  token.printOn(os);
  return os;
}



// **************************************************
//                  GenericToken methods
// **************************************************

GenericToken::GenericToken(const String &lexeme,GenericTokenType tokenType,
	     int lineNum)
  : lexeme(lexeme), tokenType(tokenType), lineNum(lineNum)

{
  // ctor
}



const String &GenericToken::getLexeme() const
{
  return lexeme;
}



GenericTokenType GenericToken::getTokenType() const
{
  return tokenType;
}



int GenericToken::tokenTypeAsInt() const
{
  return static_cast<int>(tokenType);
}



int GenericToken::getLineNum() const
{
  return lineNum;
}



void GenericToken::printOn(ostream &os) const
{
  printGenericTokenType(tokenType,os);
  os << " \t" << lexeme;
}




void GenericToken::changeLexeme(const String &newValue)
{
  lexeme=newValue;
}



bool GenericToken::isEof() 
{
  return tokenType==TOK_EOF;
}

