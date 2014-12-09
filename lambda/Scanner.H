/*
 Scanner.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_Scanner_H
#define INCL_Scanner_H
#include "Token.H"
#include <iostream>
using namespace std;
using namespace BOOM;
namespace Lambda {

/*
 A Scanner for a single file.  You must specify the
 istream in the ctor, so you can only scan one file
 with a Scanner object.  You can scan multiple files
 by stacking Scanner objects when (load ...) directives
 are encountered.
*/

class Scanner
{
  istream &is;
  int lineNum;
  bool isEofChar(int ic);
  void skipWhiteSpace();
  Token *scanStringLit();
  Token *scanCharLit();
  Token *scanSymbol();
  void skipComment();
  Token *scanGreater();
  Token *scanLess();
  Token *scanNotEqual();
  Token *scanNumber();
  Token *scanIdent();
  Token *scanMinus();
  bool validIdentChar(char);
public:
  Scanner(istream &);
  Token *nextToken();
};

}

#endif
