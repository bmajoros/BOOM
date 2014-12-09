/***********************************************************************
 DnaAlphabetWithGaps.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_DnaAlphabetWithGaps_H
#define INCL_DnaAlphabetWithGaps_H
using namespace std;
#include "Alphabet.H"


// {A,C,G,N,T,-}

namespace BOOM { 
class DnaAlphabetWithGaps : public Alphabet
{
public:
  DnaAlphabetWithGaps();
  static DnaAlphabetWithGaps global;
private:
};



class LowercaseDnaAlphabetWithGaps : public Alphabet
{
public:
  LowercaseDnaAlphabetWithGaps();
  static LowercaseDnaAlphabetWithGaps global;
private:
};
}


#endif
