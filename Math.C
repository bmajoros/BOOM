/****************************************************************
 Math.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Math.H"
using namespace std;
using namespace BOOM;

int ipow(int base, int exp)
{
  int result=1;
  while (exp) {
    if(exp & 1) result*=base;
    exp>>=1;
    base*=base;
  }
  return result;
}



