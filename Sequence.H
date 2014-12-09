/***********************************************************************
 Sequence.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Sequence_H
#define INCL_Sequence_H
using namespace std;
#include <iostream>
#include <ctype.h>
#include "String.H"
#include "Vector.H"
#include "Alphabet.H"
#include "AlphabetMap.H"
#include "Symbol.H"
#include "Regex.H"

namespace BOOM {
class Sequence
{
public:
  static Sequence *load(const BOOM::String &filename,const Alphabet &,
			BOOM::String &substrateId);
  static Sequence *load(const BOOM::String &filename,const Alphabet &);
  Sequence(const BOOM::String &,const Alphabet &);
  Sequence();
  Sequence(const char *,const Alphabet &);
  Sequence(Symbol);
  Sequence(Symbol,int repetitions);
  virtual ~Sequence() {}
  void copyFrom(const BOOM::String &,const Alphabet &);
  void resize(int length);
  void setPhase(int);
  int getPhase() const;
  void append(Symbol);
  void append(const Sequence &);
  void prepend(Symbol);
  bool operator<(const Sequence &) const;
  Sequence operator+(const Sequence &);
  Sequence operator+(Symbol);
  void operator+=(const Sequence &);
  void operator+=(Symbol);
  String operator()(const Alphabet &) const;
  Symbol operator [](int) const;
  Symbol &operator [](int);
  const Sequence &operator=(const Sequence &);
  bool operator==(const Sequence &) const;
  int getLength() const;
  void printOn(ostream &) const;
  void printOn(ostream &,const Alphabet &) const;
  BOOM::String *toString(const Alphabet &,int startingAt=0) const;
  void toString(const Alphabet &,int startingAt,int length,BOOM::String &) 
    const;
  void clear();
  virtual void getSubsequence(int begin,int len,Sequence &) const;
  bool subsequenceOccursAt(const Sequence &,int at) const;
  Sequence *reverseComplement(const Alphabet &) const;
  void reverseComplement(const Alphabet &,Sequence &) const;
  bool isEmpty() const {return symbols.isEmpty();}
  int countOccurrences(Symbol);
  bool containsSymbol(Symbol);
  void insert(Symbol,int pos);
  void insert(const Sequence &,int pos);
  void cut(int begin,int len);
  unsigned asInt(const AlphabetMap &,int begin=0,int len=-1) const; // encode
  unsigned asInt(const Alphabet &,int begin=0,int len=-1) const; // encode
  void fromInt(unsigned code,int seqLength,const AlphabetMap &); // decode
  void fromInt(unsigned code,int seqLength,const Alphabet &); // decode
  void translate(const AlphabetMap &,Sequence &into);
  void replaceAll(Symbol replaceThis,Symbol withThis);
  inline BOOM::Vector<Symbol>::iterator begin();
  inline BOOM::Vector<Symbol>::iterator end();
  void useOnlyTheseSymbols(Array1D<bool> &indexBySymbol);
private:
  static BOOM::Regex deflineRegex;
  BOOM::Vector<Symbol> symbols;
  int phase;

  void append(const Alphabet &,const char *);
};
}

ostream &operator<<(ostream &,const Sequence &);



BOOM::Vector<Symbol>::iterator Sequence::begin()
{
  return symbols.begin();
}



BOOM::Vector<Symbol>::iterator Sequence::end()
{
  return symbols.end();
}


#endif
