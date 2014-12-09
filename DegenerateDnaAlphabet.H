/***********************************************************************
 DegenerateDnaAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_DegenerateDnaAlphabet_H
#define INCL_DegenerateDnaAlphabet_H
using namespace std;
#include "DnaAlphabet.H"



/************************************************************************
    This alphabet is compatible with DnaAlphabet (ie, the assignment
    of integer codes to the symbols A,C,G,N,T is the same), but it extends
    it by adding symbols for 2-letter degenerate symbols such as purines 
    and pyrimidines, etc.
*************************************************************************/

namespace BOOM {
class DegenerateDnaAlphabet : public DnaAlphabet
{
public:
  DegenerateDnaAlphabet();
  static DegenerateDnaAlphabet global;
private:
};



class LowercaseDegenerateDnaAlphabet : public LowercaseDnaAlphabet
{
public:
  LowercaseDegenerateDnaAlphabet();
  static LowercaseDegenerateDnaAlphabet global;
private:
};
}


#endif
