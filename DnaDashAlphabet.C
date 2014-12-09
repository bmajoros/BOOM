/***********************************************************************
 DnaDashAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "DnaDashAlphabet.H"
#include <iostream>


BOOM::DnaDashAlphabet &BOOM::DnaDashAlphabet::global()
{
  static BOOM::DnaDashAlphabet g;
  return g;
}


BOOM::DnaDashAlphabet::DnaDashAlphabet()
  : Alphabet("ACGT-")
{
}



