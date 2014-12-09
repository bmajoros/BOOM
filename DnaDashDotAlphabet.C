/***********************************************************************
 DnaDashDotAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "DnaDashDotAlphabet.H"
#include <iostream>

using namespace BOOM;

DnaDashDotAlphabet::DnaDashDotAlphabet()
  : Alphabet("ACGTN-.")
{
}



DnaDashDotAlphabet &DnaDashDotAlphabet::global()
{
  static DnaDashDotAlphabet g;
  return g;
}




