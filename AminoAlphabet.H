/***********************************************************************
 AminoAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_AminoAlphabet_H
#define INCL_AminoAlphabet_H
using namespace std;
#include "Alphabet.H"

namespace BOOM {
class AminoAlphabet : public Alphabet
{
public:
  AminoAlphabet();
  static AminoAlphabet &global();
};
}


#endif
