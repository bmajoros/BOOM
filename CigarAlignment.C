/****************************************************************
 CigarAlignment.C
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "CigarAlignment.H"
using namespace std;
using namespace BOOM;

CigarAlignment::CigarAlignment(int length)
  : A(length)
{
  // ctor
}



int &CigarAlignment::operator[](int i)
{
  return A[i];
}



CigarAlignment *CigarAlignment::invert(int length)
{
  CigarAlignment &I=*new CigarAlignment(length);
  I.A.setAllTo(CIGAR_UNDEFINED);
  int L=A.size();
  for(int i=0 ; i<L ; ++i) {
    const int to=A[i];
    if(to!=CIGAR_UNDEFINED) I[to]=i;
  }
  return &I;
}


