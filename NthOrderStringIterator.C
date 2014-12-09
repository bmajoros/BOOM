/***********************************************************************
 NthOrderStringIterator.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "NthOrderStringIterator.H"
#include <iostream>
using namespace std;

NthOrderStringIterator::NthOrderStringIterator(int N,const Alphabet &alphabet)
  : alphabet(alphabet), N(N), alphabetSize(alphabet.getNumElements())
{
  // ctor

  reset();
}



BOOM::String NthOrderStringIterator::getNextString()
{
  BOOM::String *s=current.toString(alphabet);
  BOOM::String retval=*s;
  delete s;

  advance();

  return retval;
}



BOOM::Sequence NthOrderStringIterator::getNextSequence()
{
  Sequence retval=current;
  advance();
  return retval;
}



void NthOrderStringIterator::getNext(BOOM::Sequence &intoSeq,
				     BOOM::String &intoStr)
{
  BOOM::String *s=current.toString(alphabet);
  intoStr=*s;
  delete s;
  intoSeq=current;
  advance();
}



bool NthOrderStringIterator::done()
{
  return !hasMore;
}



void NthOrderStringIterator::reset()
{
  current.clear();
  Symbol z=0;
  for(int i=0 ; i<N ; ++i)
    current.append(z);
  hasMore=true;
}



void NthOrderStringIterator::advance()
{
  for(int i=N-1 ; i>=0 ; --i)
    {
      Symbol &s=current[i];
      ++s;
      if(s<alphabetSize) return;
      s=0;
    }
  hasMore=false;
}


