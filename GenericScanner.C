/****************************************************************
 GenericScanner.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <stdio.h>
#include "ElasVector.H"
#include "GenericScanner.H"
using namespace std;
using namespace BOOM;


LexicalError::LexicalError(char c,int lineNum)
  : RootException("")
{
  setMessage(String("lexical error: unexpected character '")+
	     c+"' on line "+lineNum);
}



GenericScanner::GenericScanner(istream &is)
  : is(is), lineNum(1), keywords(101)
{
  // ctor
}



GenericToken *GenericScanner::nextToken()
{
  skipWhiteSpace();

  while(!is.eof()) {
    int ic=is.get();
    if(isEofChar(ic)) break;
    char c=static_cast<char>(ic);
    switch(c) {
    case '\"': // string literal
      return scanStringLit();
    case '\'': // character literal
      return scanCharLit();
    case '*': // * or ** or *=
      return scanTimes();
    case '/': {// / or /= or comment
      GenericToken *t=scanSlash();
      if(t) return t;
      skipWhiteSpace();
      continue;}
    case '+': // +
      return scanPlus();
    case '-': // - or negative number
      return scanMinus();
    case '>': // > or >= or >>
      return scanGreater();
    case '<': // < or <= or <<
      return scanLess();
    case '=': // =
      return scanEqual();
    case '!': // ! or !=
      return scanNotEqual();
    case '~': // ~ or ~=
      return scanTilde();
    case '&': // & or && or &=
      return scanAnd();
    case '.': // .
      return new GenericToken(".",TOK_PERIOD,lineNum);
    case ',': // ,
      return new GenericToken(",",TOK_COMMA,lineNum);
    case '?': // ?
      return new GenericToken("?",TOK_QUESTION,lineNum);
    case '@': // @
      return new GenericToken("@",TOK_AT,lineNum);
    case '$': // $
      return new GenericToken("$",TOK_DOLLAR,lineNum);
    case '#': // #
      return new GenericToken("#",TOK_HASH,lineNum);
    case '^': // ^
      return new GenericToken("^",TOK_CARET,lineNum);
    case ';': // ;
      return new GenericToken(";",TOK_SEMICOLON,lineNum);
    case ':': // ;
      return new GenericToken(":",TOK_COLON,lineNum);
    case '%': // %
      return new GenericToken("%",TOK_MOD,lineNum);
    case '|': // |
      return new GenericToken("|",TOK_PIPE,lineNum);
    case '{': // {
      return new GenericToken("{",TOK_LEFT_BRACE,lineNum);
    case '}': // }
      return new GenericToken("}",TOK_RIGHT_BRACE,lineNum);
    case '[': // [
      return new GenericToken("[",TOK_LEFT_BRACKET,lineNum);
    case ']': // ]
      return new GenericToken("]",TOK_RIGHT_BRACKET,lineNum);
    case '(': // (
      return new GenericToken("(",TOK_LEFT_PAREN,lineNum);
    case ')': // )
      return new GenericToken(")",TOK_RIGHT_PAREN,lineNum);
    default:
      if(isdigit(c)) {
	is.putback(c);
	return scanNumber();
      }
      else if(isalpha(c) || c=='_') {
	is.putback(c);
	return scanIdent();
      }
      throw LexicalError(c,lineNum);
    }
  }
  return new GenericToken("",TOK_EOF,lineNum);
}



void GenericScanner::skipWhiteSpace()
{
  // This method skips whitespace and keeps count of newlines
  // in the process
  
  int ic;
  char c=' ';
  while(isspace(c)) {
    ic=is.get();
    if(isEofChar(ic)) return;
    c=char(ic);
    if(c=='\r' || c=='\n') ++lineNum;
  }
  is.putback(c);
}



bool GenericScanner::isEofChar(int ic) 
{ 
  return ic==EOF || ic==0; 
}



GenericToken *GenericScanner::scanMinus()
{
  // Precondition: a '-' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  
  if(isEofChar(ic))
    return new GenericToken("-",TOK_MINUS,lineNum);
  
  if(isdigit(c)) {
    is.putback(c);
    GenericToken *token=scanNumber();
    const String &lexeme=token->getLexeme();
    if(lexeme.contains('.')) { // float
      float value=lexeme.asFloat();
      value *= -1;
      token->changeLexeme(String(value));
    }
    else { // int
      int value=lexeme.asInt();
      value *= -1;
      token->changeLexeme(String((long)value));
    }
    return token;
  }
  else if(c=='-') return new GenericToken("--",TOK_DECREMENT,lineNum);
  else if(c=='=') return new GenericToken("-=",TOK_MINUS_EQUAL,lineNum);
  else if(c=='>') return new GenericToken("->",TOK_ARROW,lineNum);

  is.putback(c);
  return new GenericToken("-",TOK_MINUS,lineNum);
}



GenericToken *GenericScanner::scanNumber()
{ 
  char buffer[256];
  int i=0;
  bool periodSeen=false;

  while(!is.eof() && i<255) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) break;
    if(!(isdigit(c) || c=='.' || c=='e' || c=='-' || c=='+')) {
      is.putback(c);
      break;
    }
    if(c=='e') {
      if(i==0) INTERNAL_ERROR;
      if(!isdigit(buffer[i-1])) { is.putback(c); break; } }
    if(c=='+' || c=='-') {
      if(i==0) INTERNAL_ERROR;
      if(buffer[i-1]!='e') { is.putback(c); break; } }
    buffer[i]=c;
    ++i;
    if(c=='.')
      if(periodSeen) throw LexicalError(c,lineNum);
      else periodSeen=true;
  }
  buffer[i]='\0';
  GenericTokenType tokenType=periodSeen ? TOK_FLOAT_LIT : TOK_INT_LIT;
  return new GenericToken(buffer,tokenType,lineNum);
}



bool GenericScanner::validIdentChar(char c)
{
  return  isalpha(c) || isdigit(c) || c=='_' || c=='-' || c=='?';
}



GenericToken *GenericScanner::scanIdent()
{
  char buffer[256];
  int i=0;
  while(!is.eof() && i<255) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) break;
    if(!validIdentChar(c)) {
      is.putback(c);
      break;
    }
    buffer[i]=c;
    ++i;
  }
  buffer[i]='\0';
  if(keywords.isDefined(buffer,i))
    return new GenericToken(buffer,
      static_cast<GenericTokenType>(keywords.lookup(buffer,i)),lineNum);
  return new GenericToken(buffer,TOK_IDENT,lineNum);
}



GenericToken *GenericScanner::scanStringLit()
{
  // Precondition: A '\"' has already been read

  ElasCharVect array;
  int i=0;

  // Read string literal in to elastic array
  while(!is.eof()) {
    int ic=is.get();
    char c=static_cast<char>(ic);
    if(isEofChar(ic)) throw LexicalError(c,lineNum);
    if(c=='\"') break;
    if(c=='\\') {
      ic=is.get();
      c=static_cast<char>(ic);
      if(isEofChar(ic)) throw LexicalError(c,lineNum);
      switch(c)	{
      case 'n':	c='\n';	break;
      case 'r':	c='\r';	break;
      case 't':	c='\t';	break;
      }
    }
    array[i]=c;
    ++i;
  }
  const int len=i;

  // Copy elastic array into a char string
  char *buffer=new char[len+1];
  for(i=0 ; i<len ; ++i) buffer[i]=array[i];
  buffer[i]='\0';

  // Create token
  GenericToken *token=new GenericToken(buffer,TOK_STRING_LIT,lineNum);
  
  // Clean up and return
  delete [] buffer;
  return token;
}



GenericToken *GenericScanner::scanCharLit()
{
  // Precondition: A ' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(isEofChar(ic)) throw LexicalError(c,lineNum);

  char buffer[2];
  buffer[1]='\0';

  if(c=='\\') {
    ic=is.get();
    c=static_cast<char>(ic);
    switch(c) {
    case 'n':      buffer[0]='\n';   break;
    case 'r':      buffer[0]='\r';   break;
    case 't':      buffer[0]='\t';   break;
    case '\\':     buffer[0]=c;      break;
    case '\'':     buffer[0]=c;      break;
    case '\"':     buffer[0]=c;      break;
    }
  }
  else buffer[0]=c;

  // Match the final '
  ic=is.get();
  c=static_cast<char>(ic);
  if(c!='\'') throw LexicalError(c,lineNum);
  return new GenericToken(buffer,TOK_CHAR_LIT,lineNum);
}



GenericToken *GenericScanner::scanGreater()
{
  // Precondition: A '>' has already been read

  // Could be > or >= or >>

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=') return new GenericToken(">=",TOK_GREATER_EQUAL,lineNum);
  if(c=='>') return new GenericToken(">>",TOK_SHIFT_RIGHT,lineNum);
  is.putback(c);
  return new GenericToken(">",TOK_GREATER,lineNum);
}



GenericToken *GenericScanner::scanLess()
{
  // Precondition: A '<' has already been read

  // Could be < or <= or <<

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=') return new GenericToken("<=",TOK_LESS_EQUAL,lineNum);
  if(c=='<') return new GenericToken("<<",TOK_SHIFT_LEFT,lineNum);
  is.putback(c);
  return new GenericToken("<",TOK_LESS,lineNum);
}



GenericToken *GenericScanner::scanNotEqual()
{
  // Precondition: A '!' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=') return new GenericToken("!=",TOK_NOT_EQUAL,lineNum);
  is.putback(c);
  return new GenericToken("!",TOK_NOT,lineNum);
}



GenericToken *GenericScanner::scanTimes()
{
  // Precondition: A '*' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=') return new GenericToken("*=",TOK_TIMES_EQUAL,lineNum);
  else if(c=='*') return new GenericToken("**",TOK_DOUBLE_STAR,lineNum);
  is.putback(c);
  return new GenericToken("*",TOK_TIMES,lineNum);
}



GenericToken *GenericScanner::scanSlash()
{
  // Precondition: A '/' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=') return new GenericToken("/=",TOK_DIVIDE_EQUAL,lineNum);
  else if(c=='/') {
    while(!is.eof()) {
      int ic=is.get();
      char c=static_cast<char>(ic);
      if(isEofChar(ic) || c=='\n') break;
    }
    ++lineNum;
    return NULL;
  }
  is.putback(c);
  return new GenericToken("/",TOK_DIVIDE,lineNum);
}



GenericToken *GenericScanner::scanPlus()
{
  // Precondition: a '+' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  
  if(isEofChar(ic)) return new GenericToken("+",TOK_PLUS,lineNum);
  
  if(c=='=') return new GenericToken("+=",TOK_PLUS_EQUAL,lineNum);
  else if(c=='+') return new GenericToken("++",TOK_INCREMENT,lineNum);
  is.putback(c);
  return new GenericToken("+",TOK_PLUS,lineNum);
}



GenericToken *GenericScanner::scanEqual()
{
  // Precondition: a '=' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  
  if(isEofChar(ic)) return new GenericToken("=",TOK_EQUAL,lineNum);
  
  if(c=='=') return new GenericToken("==",TOK_IS_EQUAL,lineNum);
  else if(c=='>') return new GenericToken("=>",TOK_DOUBLE_ARROW,lineNum);
  is.putback(c);
  return new GenericToken("=",TOK_EQUAL,lineNum);
}



GenericToken *GenericScanner::scanTilde()
{
  // Precondition: a '~' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  
  if(isEofChar(ic)) return new GenericToken("~",TOK_TILDE,lineNum);
  
  if(c=='=') return new GenericToken("~=",TOK_TILDE_EQUAL,lineNum);
  is.putback(c);
  return new GenericToken("~",TOK_TILDE,lineNum);
}



GenericToken *GenericScanner::scanAnd()
{
  // Precondition: a '&' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  
  if(isEofChar(ic)) return new GenericToken("&",TOK_AND,lineNum);
  
  if(c=='=') return new GenericToken("&=",TOK_AND_EQUAL,lineNum);
  if(c=='&') return new GenericToken("&&",TOK_DOUBLE_AND,lineNum);
  is.putback(c);
  return new GenericToken("&",TOK_AND,lineNum);
}



void GenericScanner::registerKeyword(const String &lexeme,int tokenType)
{
  keywords.lookup(lexeme)=tokenType;
}


