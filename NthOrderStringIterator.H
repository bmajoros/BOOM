/***********************************************************************
 NthOrderStringIterator.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_NthOrder_StringIterator_H
#define INCL_NthOrder_StringIterator_H
#include "Alphabet.H"
#include "Sequence.H"
#include "String.H"

// Generates strings of length N (not N+1!)

namespace BOOM {
class NthOrderStringIterator
{
  const Alphabet &alphabet;
  int alphabetSize, N;
  Sequence current;
  bool hasMore;

  void advance();
public:
  NthOrderStringIterator(int N,const Alphabet &);
  BOOM::String getNextString();
  BOOM::Sequence getNextSequence();
  void getNext(BOOM::Sequence &,BOOM::String &);
  bool done();
  void reset();
};
}

#endif
