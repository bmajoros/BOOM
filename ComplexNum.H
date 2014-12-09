/***********************************************************************
 ComplexNum.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_ComplexNum_H
#define INCL_BOOM_ComplexNum_H
using namespace std;

namespace BOOM {
class ComplexNum
{
public:
  ComplexNum(double real=0.0,double imag=0.0);
  ComplexNum operator+(const BOOM::ComplexNum &);
  ComplexNum operator*(const BOOM::ComplexNum &);
  ComplexNum getConjugate();
  double &getImag();
  double &getReal();
  double getImag() const;
  double getModulus() const;
  double getReal() const;
private:
  double real, imag;
};
}

#endif
