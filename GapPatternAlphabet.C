/***********************************************************************
 GapPatternAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "GapPatternAlphabet.H"
#include <iostream>



BOOM::GapPatternAlphabet::GapPatternAlphabet()
  : Alphabet("N-.")
{
}



BOOM::GapPatternAlphabet &BOOM::GapPatternAlphabet::global()
{
  static GapPatternAlphabet a;
  return a;
}


