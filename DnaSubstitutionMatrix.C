/****************************************************************
 DnaSubstitutionMatrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "DnaSubstitutionMatrix.H"
#include "DnaAlphabet.H"
using namespace std;
using namespace BOOM;

DnaSubstitutionMatrix::DnaSubstitutionMatrix()
  : SubstitutionMatrix<float>(DnaAlphabet::global())
{
    // ctor

    int n=alphabet.getNumElements();
    for(int i=0 ; i<n ; ++i)
        for(int j=0 ; j<n ; ++j)
            if(i==j)
                set(i,j,1.0);
            else
                set(i,j,0.0);
    
    computeMeanValue(-1);
    computePercentOverZero(-1);
}


