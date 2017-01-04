/***********************************************************************
 Alphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Alphabet_H
#define INCL_Alphabet_H
using namespace std;
#include "Symbol.H"
#include <iostream>

namespace BOOM {
class Alphabet {
public:
  Alphabet(const char * = NULL);
  int add(char);
  bool load(istream &);
  int getNumElements() const;
  int size() const {return numElements;}
  inline char lookup(int) const;
  inline int lookup(char) const;
  void printOn(ostream &);
  char complement(char) const;
  Symbol complement(Symbol) const;
  bool save(ostream &);
  bool isDefined(char) const;
private:
  char intToChar[256]; // int->char
  char charToInt[256]; // char->int
  int numElements;
};
}
using namespace BOOM;


ostream &operator<<(ostream &,Alphabet &);



char BOOM::Alphabet::lookup(int index) const
{
  //return intToChar[index];
  return index>=0 ? intToChar[index] : '$';
}



int BOOM::Alphabet::lookup(char c) const
{
  return static_cast<int>(charToInt[static_cast<int>(c)]);
}

#endif
