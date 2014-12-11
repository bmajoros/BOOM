/****************************************************************
 GenericTokenStream.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GenericTokenStream_H
#define INCL_GenericTokenStream_H
#include <iostream>
#include "GenericScanner.H"
using namespace std;
using namespace BOOM;

namespace BOOM {
class GenericTokenStream {
  GenericScanner &scanner;
  GenericToken *pushedBack;
public:
  GenericTokenStream(GenericScanner *);
  GenericTokenStream(istream &);
  GenericToken *nextToken();
  void pushBack(GenericToken *);
  GenericTokenType peekTokenType();
  GenericScanner &getScanner();
};
}

#endif

