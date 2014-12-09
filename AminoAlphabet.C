/***********************************************************************
 AminoAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "AminoAlphabet.H"
#include <iostream>


BOOM::AminoAlphabet::AminoAlphabet()
  : Alphabet("*ARNDCQEGHILKMFPSTWYV") // plus BZX ...
{
}



AminoAlphabet &AminoAlphabet::global()
{
  static AminoAlphabet a;
  return a;
}


