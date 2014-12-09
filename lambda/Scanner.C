/*
 Scanner.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Scanner.H"
#include <ctype.h>
#include "LambdaException.H"
#include "BOOM/ElasVector.H"
using namespace BOOM;
using namespace Lambda;


Scanner::Scanner(istream &is)
  : is(is), lineNum(1)
{
  // ctor
}


void Scanner::skipWhiteSpace()
{
  // This method skips whitespace and keeps count of newlines
  // in the process
  
  int ic;
  char c=' ';
  while(isspace(c))
    {
      ic=is.get();
      if(isEofChar(ic)) return;
      c=char(ic);
      if(c=='\r' || c=='\n') ++lineNum;
    }
  is.putback(c);
}



bool Scanner::isEofChar(int ic) 
{ 
  return ic==EOF || ic==0; 
}



Token *Scanner::nextToken()
{
  skipWhiteSpace();

  while(!is.eof())
    {
      int ic=is.get();
      if(isEofChar(ic)) break;
      char c=static_cast<char>(ic);
      switch(c)
	{
	case '\"': // string literal
	  return scanStringLit();
	case '#': // character literal
	  return scanCharLit();
	case '\'': // symbol
	  return scanSymbol();
	case '*': // *
	  return new Token("*",TOK_TIMES,lineNum);
	case '/': // /
	  return new Token("/",TOK_DIVIDE,lineNum);
	case '%': // %
	  return new Token("%",TOK_MOD,lineNum);
	case '+': // +
	  return new Token("+",TOK_PLUS,lineNum);
	case '-': // - or negative number
	  return scanMinus();
	case ';': // comment
	  skipComment();
	  skipWhiteSpace();
	  continue;
	case '>': // > or >=
	  return scanGreater();
	  break;
	case '<': // < or <=
	  return scanLess();
	  break;
	case '=': // =
	  return new Token("=",TOK_EQUAL,lineNum);
	case '!': // !=
	  return scanNotEqual();
	  break;
	case '|': // |
	  return new Token("|",TOK_PIPE,lineNum);
	case '[': // [
	  return new Token("[",TOK_LEFT_BRACKET,lineNum);
	case ']': // ]
	  return new Token("]",TOK_RIGHT_BRACKET,lineNum);
	case '(': // (
	  return new Token("(",TOK_LEFT_PAREN,lineNum);
	case ')': // )
	  return new Token(")",TOK_RIGHT_PAREN,lineNum);
	default:
	  if(isdigit(c))
	    {
	      is.putback(c);
	      return scanNumber();
	    }
	  else if(isalpha(c) || c=='_')
	    {
	      is.putback(c);
	      return scanIdent();
	    }
	  throw LexicalError(c,lineNum);
	}
    }

  return new Token("",TOK_EOF,lineNum);
}



Token *Scanner::scanMinus()
{
  // Precondition: a '-' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  
  if(isEofChar(ic))
    return new Token("-",TOK_MINUS,lineNum);
  
  if(isdigit(c))
    {
      is.putback(c);
      Token *token=scanNumber();
      const String &lexeme=token->getLexeme();
      if(lexeme.contains('.'))
	{ // float
	  float value=lexeme.asFloat();
	  value *= -1;
	  token->changeLexeme(String(value));
	}
      else
	{ // int
	  int value=lexeme.asInt();
	  value *= -1;
	  token->changeLexeme(String((long)value));
	}
      return token;
    }
  else
    {
      is.putback(c);
      return new Token("-",TOK_MINUS,lineNum);
    }
}



Token *Scanner::scanNumber()
{ 
  char buffer[256];
  int i=0;
  bool periodSeen=false;

  while(!is.eof() && i<255)
    {
      int ic=is.get();
      char c=static_cast<char>(ic);
      
      if(isEofChar(ic)) break;
      if(!(isdigit(c) || c=='.'))
	{
	  is.putback(c);
	  break;
	}

      buffer[i]=c;
      ++i;

      if(c=='.')
	if(periodSeen)
	  throw LexicalError(c,lineNum);
	else
	  periodSeen=true;
    }

  buffer[i]='\0';

  TokenType tokenType=periodSeen ? TOK_FLOAT_LIT : TOK_INT_LIT;
  return new Token(buffer,tokenType,lineNum);
}



bool Scanner::validIdentChar(char c)
{
  return 
    isalpha(c) || isdigit(c) || c=='_' || c=='-'
    || c=='?';
}



Token *Scanner::scanIdent()
{
  char buffer[256];
  int i=0;

  while(!is.eof() && i<255)
    {
      int ic=is.get();
      char c=static_cast<char>(ic);
      
      if(isEofChar(ic)) break;
      if(!validIdentChar(c))
	{
	  is.putback(c);
	  break;
	}

      buffer[i]=c;
      ++i;
    }

  buffer[i]='\0';

  return new Token(buffer,TOK_IDENT,lineNum);
}



Token *Scanner::scanStringLit()
{
  // Precondition: A '\"' has already been read

  ElasCharVect array;
  int i=0;

  // Read string literal in to elastic array
  while(!is.eof())
    {
      int ic=is.get();
      char c=static_cast<char>(ic);
      if(isEofChar(ic))
	throw LexicalError(c,lineNum);
      if(c=='\"') break;
      if(c=='\\')
	{
	  ic=is.get();
	  c=static_cast<char>(ic);
	  if(isEofChar(ic))
	    throw LexicalError(c,lineNum);
	  switch(c)
	    {
	    case 'n':
	      c='\n';
	      break;
	    case 'r':
	      c='\r';
	      break;
	    case 't':
	      c='\t';
	      break;
	    }
	}
      array[i]=c;
      ++i;
    }
  const int len=i;

  // Copy elastic array into a char string
  char *buffer=new char[len+1];
  for(i=0 ; i<len ; ++i)
    buffer[i]=array[i];
  buffer[i]='\0';

  // Create token
  Token *token=new Token(buffer,TOK_STRING_LIT,lineNum);
  
  // Clean up and return
  delete [] buffer;
  return token;
}



Token *Scanner::scanCharLit()
{
  // Precondition: A '#' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(isEofChar(ic))
    throw LexicalError(c,lineNum);

  char buffer[2];
  buffer[1]='\0';

  if(c=='\\')
    {
      ic=is.get();
      c=static_cast<char>(ic);
      switch(c)
	{
	case 'n':
	  buffer[0]='\n';
	  break;
	case 'r':
	  buffer[0]='\r';
	  break;
	case 't':
	  buffer[0]='\t';
	  break;
	case '\\':
	  buffer[0]=c;
	  break;
	case '\'':
	  buffer[0]=c;
	  break;
	case '\"':
	  buffer[0]=c;
	  break;
	}
    }
  else
    buffer[0]=c;
    
  return new Token(buffer,TOK_CHAR_LIT,lineNum);
}



Token *Scanner::scanSymbol()
{
  // Precondition: A '\'' has already been read

  Token *temp=scanIdent();
  Token *symbol=
    new Token(temp->getLexeme(),TOK_SYMBOL,temp->getLineNum());
  delete temp;
  return symbol;
}



void Scanner::skipComment()
{
  // Precondition: A ';' has already been read

  while(!is.eof())
    {
      int ic=is.get();
      char c=static_cast<char>(ic);
      if(isEofChar(ic) || c=='\n') break;
    }

  ++lineNum;
}



Token *Scanner::scanGreater()
{
  // Precondition: A '>' has already been read

  // Could be > or >=

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=')
    return new Token(">=",TOK_GREATER_EQUAL,lineNum);
  is.putback(c);
  return new Token(">",TOK_GREATER,lineNum);
}



Token *Scanner::scanLess()
{
  // Precondition: A '<' has already been read

  // Could be < or <=

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c=='=')
    return new Token("<=",TOK_LESS_EQUAL,lineNum);
  is.putback(c);
  return new Token("<",TOK_LESS,lineNum);
}



Token *Scanner::scanNotEqual()
{
  // Precondition: A '!' has already been read

  int ic=is.get();
  char c=static_cast<char>(ic);
  if(c!='=') 
    throw LexicalError(c,lineNum);
  return new Token("!=",TOK_NOT_EQUAL,lineNum);
}


