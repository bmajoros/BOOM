/***********************************************************************
 StrTokenizer.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_StrTokenizer_H
#define INCL_BOOM_StrTokenizer_H
#include "Vector.H"
#include "String.H"
using namespace std;
namespace BOOM {

#define DEFAULT_WHITESPACE " \t\n\r"

typedef pair< BOOM::Vector<BOOM::String>, 
	      BOOM::Vector<BOOM::String> > StrVectPair;

class StrTokenizer 
{
public:
  StrTokenizer(const char *wspace=DEFAULT_WHITESPACE);
  StrTokenizer(const char *source,
		  const char *wspace=DEFAULT_WHITESPACE);
  StrTokenizer(const BOOM::String &source,
		  const char *wspace=DEFAULT_WHITESPACE);
  BOOM::Vector<BOOM::String*> *getAllTokens();
  BOOM::Vector<BOOM::String> *getTokenStrings();
  bool hasMoreTokens();
  const char *nextToken();
  StrVectPair *getTokensAndSeparators(const BOOM::String &);
  static BOOM::Vector<BOOM::String*> *tokenize(const char *source,
		  const char *wspace=DEFAULT_WHITESPACE);
  void tokenizeThis(const BOOM::String &s);
  void tokenizeThis(const char *p);
private:
  enum WhereType 
    { 
      IN_TOKEN, 
      IN_SPACE, 
      AT_START 
    };
  const char *p;
  const char *source;
  const char *wspace;
  BOOM::String buffer;
  bool isWhiteSpace(char c);
  void skipWhiteSpace();
};
}

#endif
