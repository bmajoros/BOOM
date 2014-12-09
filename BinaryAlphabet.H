/****************************************************************
 BinaryAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_BinaryAlphabet_H
#define INCL_BinaryAlphabet_H
#include <iostream>
#include "Alphabet.H"
using namespace std;
using namespace BOOM;


class BinaryAlphabet : public Alphabet {
public:
  BinaryAlphabet();
  static BinaryAlphabet &global();
};

#endif

