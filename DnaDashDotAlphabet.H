/***********************************************************************
 DnaDashDotAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_DnaDashDotAlphabet_H
#define INCL_DnaDashDotAlphabet_H
using namespace std;
#include "Alphabet.H"


// {A,C,G,T,N,-,.}

namespace BOOM { 
class DnaDashDotAlphabet : public Alphabet
{
public:
  DnaDashDotAlphabet();
  static DnaDashDotAlphabet &global();
};

} // end namespace BOOM


#endif
