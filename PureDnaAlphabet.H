/***********************************************************************
 PureDnaAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_PureDnaAlphabet_H
#define INCL_PureDnaAlphabet_H
using namespace std;
#include "Alphabet.H"


/***********************************************************************
 Like DnaAlphabet, but without the 'N' character (i.e., just ACGT)
 ***********************************************************************/

namespace BOOM {
class PureDnaAlphabet : public Alphabet
{
public:
  PureDnaAlphabet();
  static PureDnaAlphabet &global();
private:
};



class LowercasePureDnaAlphabet : public Alphabet
{
public:
  LowercasePureDnaAlphabet();
  static LowercasePureDnaAlphabet &global();
private:
};
}


#endif
