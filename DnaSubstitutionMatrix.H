/****************************************************************
 DnaSubstitutionMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_DnaSubstitutionMatrix_H
#define INCL_DnaSubstitutionMatrix_H
#include <iostream>
#include "SubstitutionMatrix.H"
using namespace std;


namespace BOOM {

class DnaSubstitutionMatrix : public SubstitutionMatrix<float>
{
public:
  DnaSubstitutionMatrix();
};
}
#endif

