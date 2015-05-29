/****************************************************************
 CigarAlignment.H
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_CigarAlignment_H
#define INCL_CigarAlignment_H
#include <iostream>
#include "Array1D.H"
using namespace std;
namespace BOOM {

const int CIGAR_UNDEFINED=-1;

class CigarAlignment {
public:
  CigarAlignment(int length);
  int &operator[](int);
  int operator[](int) const;
  CigarAlignment *invert(int length) const;
  int length() const;
private:
  Array1D<int> A;
};
}
#endif

