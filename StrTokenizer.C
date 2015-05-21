/***********************************************************************
 StrTokenizer.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <ctype.h>
#include "String.H"
#include "StrTokenizer.H"
using namespace std;
using namespace BOOM;


BOOM::StrTokenizer::StrTokenizer(const char *wspace)
  : p(NULL), source(NULL), wspace(wspace)
{
}



BOOM::StrTokenizer::StrTokenizer(const char *src,
				   const char *wspace)
  : wspace(wspace), source(src), p(src)
{
}



BOOM::StrTokenizer::StrTokenizer(const BOOM::String &src,
				   const char *wspace)
  : wspace(wspace), source(src.c_str()), p(src.c_str())
{
}



BOOM::Vector<BOOM::String*> *BOOM::StrTokenizer::getAllTokens()
{
  BOOM::Vector<BOOM::String*> &toks=
    *new BOOM::Vector<BOOM::String*>;
  while(hasMoreTokens())
    {
      BOOM::String *s=new BOOM::String(nextToken());
      toks.push_back(s);
    }
  return &toks;
}



BOOM::Vector<BOOM::String*> *BOOM::StrTokenizer::tokenize(const char *src,
						    const char *wspace)
{
  BOOM::StrTokenizer tokenizer(src,wspace);
  BOOM::Vector<BOOM::String*> *toks=tokenizer.getAllTokens();
  return toks;
}



void BOOM::StrTokenizer::getTokenStrings(BOOM::Vector<BOOM::String> &into)
{
  while(hasMoreTokens()) into.push_back(BOOM::String(nextToken()));
}



BOOM::Vector<BOOM::String> *BOOM::StrTokenizer::getTokenStrings()
{
  BOOM::Vector<BOOM::String> &toks=
    *new BOOM::Vector<BOOM::String>;
  while(hasMoreTokens())
    toks.push_back(BOOM::String(nextToken()));
  return &toks;
}



bool BOOM::StrTokenizer::hasMoreTokens()
{
  skipWhiteSpace();
  return p && (*p!='\0');
}



bool BOOM::StrTokenizer::isWhiteSpace(char c)
{
  if(!*wspace) return !isalpha(c);

  const char *wptr=wspace;
  while(*wptr)
    {
      if(*wptr==c) return true;
      ++wptr;
    }
  return false;
}



const char *BOOM::StrTokenizer::nextToken()
{
  skipWhiteSpace();
  buffer="";
  while(*p && !isWhiteSpace(*p))
    {
      buffer+=*p;
      ++p;
    }
  const char *rval=buffer.c_str();
  return rval;
}



BOOM::StrVectPair* BOOM::StrTokenizer::getTokensAndSeparators(
  const BOOM::String &src)
{
  WhereType state=AT_START;
  BOOM::String spc, tok;
  StrVectPair &thePair=*new StrVectPair;
  for(const char *ptr=src.c_str() ; *ptr ; ++ptr)
    {
      if(!isWhiteSpace(*ptr))
	{
	  if(state==IN_SPACE)
	    {
	      thePair.second.push_back(spc);
	      spc="";
	    }
	  tok+=*ptr;
	  state=IN_TOKEN;
	}
      else
	{
	  if(state==IN_TOKEN)
	    {
	      thePair.first.push_back(tok);
	      tok="";
	    }
	  spc+=*ptr;
	  state=IN_SPACE;
	}
    }

  if(tok.length()) thePair.first.push_back(tok);
  if(spc.length()) thePair.second.push_back(spc);

  return &thePair;
}



void BOOM::StrTokenizer::skipWhiteSpace()
{
  while(*p && isWhiteSpace(*p)) p++;
}



void BOOM::StrTokenizer::tokenizeThis(const BOOM::String &str)
{
  p=str.c_str();
  source=str.c_str();
}



void BOOM::StrTokenizer::tokenizeThis(const char *str)
{
  p=str;
  source=str;
}
