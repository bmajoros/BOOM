/***********************************************************************
 DegenerateDnaAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "DegenerateDnaAlphabet.H"
#include <iostream>



BOOM::DegenerateDnaAlphabet BOOM::DegenerateDnaAlphabet::global;
BOOM::LowercaseDegenerateDnaAlphabet 
  BOOM::LowercaseDegenerateDnaAlphabet::global;


BOOM::DegenerateDnaAlphabet::DegenerateDnaAlphabet()
{
  add('R');
  add('Y');
  add('W');
  add('S');
  add('K');
  add('M');
}



BOOM::LowercaseDegenerateDnaAlphabet::LowercaseDegenerateDnaAlphabet()
{
  add('r');
  add('y');
  add('w');
  add('s');
  add('k');
  add('m');
}

