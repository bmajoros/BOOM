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



int CigarAlignment::operator[](int i) const
{
  return A[i];
}



int CigarAlignment::mapApproximate(int i,Direction prefer) const
{
  const int L=A.size();
  switch(prefer) {
  case DIR_LEFT:
    for(int j=i ; j>=0 ; --j)
      if(A[j]!=CIGAR_UNDEFINED) return A[j];
    for(int j=i+1 ; j<L ; ++j)
      if(A[j]!=CIGAR_UNDEFINED) return A[j];
    break;
  case DIR_RIGHT:
    for(int j=i+1 ; j<L ; ++j)
      if(A[j]!=CIGAR_UNDEFINED) return A[j];
   for(int j=i ; j>=0 ; --j)
      if(A[j]!=CIGAR_UNDEFINED) return A[j];
  case DIR_NONE:
    for(int j=i, k=i ; j>=0 || k<L ; --j, ++k) {
      
    }
  default: throw "Bad direction in CigarAlignment::mapApproximate()";
  }
  return CIGAR_UNDEFINED;
}



CigarAlignment *CigarAlignment::invert(int length) const
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



int CigarAlignment::length() const
{
  return A.size();
}


