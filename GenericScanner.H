/****************************************************************
 GenericScanner.H
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GenericScanner_H
#define INCL_GenericScanner_H
#include <iostream>
#include "StringMap.H"
#include "Exceptions.H"
#include "GenericToken.H"
using namespace std;
namespace BOOM {

class LexicalError : public RootException {
public:
  LexicalError(char,int lineNum);
};


/**************************************************
 A Scanner for a single file.  You must specify the
 istream in the ctor, so you can only scan one file
 with a Scanner object.  You can scan multiple files
 by stacking Scanner objects when (load ...) directives
 are encountered.

 NOTE: comments are C++ style:   // ...comment...
*/

class GenericScanner {
  istream &is;
  int lineNum;
  bool isEofChar(int ic);
  void skipWhiteSpace();
  StringMap<int> keywords;
  GenericToken *scanStringLit();
  GenericToken *scanCharLit();
  GenericToken *scanGreater();  // > or >= or >>
  GenericToken *scanLess();     // < or <= or <<
  GenericToken *scanNotEqual(); // ! or !=
  GenericToken *scanNumber();
  GenericToken *scanIdent();
  GenericToken *scanMinus(); //   - or -= or --
  GenericToken *scanPlus();  //   + or += or ++
  GenericToken *scanTimes(); //   * or ** or *=
  GenericToken *scanSlash(); //   / or /= or //comment
  GenericToken *scanEqual(); //   = or == or =>
  GenericToken *scanTilde(); //   ~ or ~=
  GenericToken *scanAnd();   //   & or && or &=
  bool validIdentChar(char);
public:
  GenericScanner(istream &);
  void registerKeyword(const String &lexeme,int tokenType);// > TOK_KEYWORD
  GenericToken *nextToken();
};


}
#endif

