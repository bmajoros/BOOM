/***********************************************************************
 PureDnaAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "PureDnaAlphabet.H"
#include <iostream>
using namespace BOOM;


PureDnaAlphabet::PureDnaAlphabet()
  : Alphabet("ACGT")
{
}



PureDnaAlphabet &PureDnaAlphabet::global()
{
  static PureDnaAlphabet m;
  return m;
}



LowercasePureDnaAlphabet::LowercasePureDnaAlphabet()
  : Alphabet("acgt")
{
}



LowercasePureDnaAlphabet &LowercasePureDnaAlphabet::global()
{
  static LowercasePureDnaAlphabet m;
  return m;
}

