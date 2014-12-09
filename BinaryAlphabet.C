/****************************************************************
 BinaryAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BinaryAlphabet.H"
using namespace std;
using namespace BOOM;


BinaryAlphabet::BinaryAlphabet()
  : Alphabet("01")
{
  // ctor
}


BinaryAlphabet &BinaryAlphabet::global()
{
  static BinaryAlphabet g;
  return g;
}

