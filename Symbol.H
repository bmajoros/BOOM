/***********************************************************************
 Symbol.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Boom_Symbol_H
#define INCL_Boom_Symbol_H
using namespace std;

namespace BOOM {

typedef int NmerSymbol;

class Symbol
{
  unsigned char alphabetIndex;
public:
  Symbol(int =0);
  bool operator==(const Symbol &);
  bool operator!=(const Symbol &s) {return !(*this==s);}
  operator int() const;
  int getIndex() const;
  Symbol &operator++();
};

#define INVALID_SYMBOL BOOM::Symbol(255)


}

#endif
