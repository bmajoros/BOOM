/***********************************************************************
 DnaAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_DnaAlphabet_H
#define INCL_DnaAlphabet_H
using namespace std;
#include "Alphabet.H"

namespace BOOM {
class DnaAlphabet : public Alphabet
{
public:
  DnaAlphabet();
  static DnaAlphabet &global();
private:
};



class LowercaseDnaAlphabet : public Alphabet
{
public:
  LowercaseDnaAlphabet();
  static LowercaseDnaAlphabet &global();
private:
};
}


#endif
