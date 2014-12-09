/***********************************************************************
 DnaAlphabetWithGaps.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "DnaAlphabetWithGaps.H"
#include <iostream>

BOOM::DnaAlphabetWithGaps BOOM::DnaAlphabetWithGaps::global;
BOOM::LowercaseDnaAlphabetWithGaps BOOM::LowercaseDnaAlphabetWithGaps::global;


BOOM::DnaAlphabetWithGaps::DnaAlphabetWithGaps()
  : Alphabet("ACGNT-")
{
}



BOOM::LowercaseDnaAlphabetWithGaps::LowercaseDnaAlphabetWithGaps()
  : Alphabet("acgnt-")
{
}

