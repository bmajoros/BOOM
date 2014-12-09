/***********************************************************************
 DnaAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "DnaAlphabet.H"
#include <iostream>

DnaAlphabet &DnaAlphabet::global()
{
  static DnaAlphabet a;
  return a;
}


LowercaseDnaAlphabet &LowercaseDnaAlphabet::global()
{
  static LowercaseDnaAlphabet a;
  return a;
}


DnaAlphabet::DnaAlphabet()
  : Alphabet("ACGNT")
{
}



LowercaseDnaAlphabet::LowercaseDnaAlphabet()
  : Alphabet("acgnt")
{
}

