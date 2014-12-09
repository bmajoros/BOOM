/***********************************************************************
 GapPatternAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_GapPatternAlphabet_H
#define INCL_GapPatternAlphabet_H
using namespace std;
#include "Alphabet.H"


// {N,-,.}

namespace BOOM { 
class GapPatternAlphabet : public Alphabet
{
public:
  GapPatternAlphabet();
  static GapPatternAlphabet &global();
};

} // end namespace BOOM


#endif
