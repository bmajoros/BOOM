/***********************************************************************
 ComplexNum.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include <math.h>
#include "ComplexNum.H"
using namespace std;



BOOM::ComplexNum::ComplexNum(double real,double imag)
  : real(real), imag(imag)
{
   
}



BOOM::ComplexNum BOOM::ComplexNum::operator+(const BOOM::ComplexNum &other)
{
  return BOOM::ComplexNum(real+other.real,imag+other.imag);
}



BOOM::ComplexNum BOOM::ComplexNum::operator*(const BOOM::ComplexNum &other)
{
  double a=real, b=imag, c=other.real, d=other.imag;
  return BOOM::ComplexNum(a*c-b*d,a*d+c*b);
}



BOOM::ComplexNum BOOM::ComplexNum::getConjugate()
{
  return BOOM::ComplexNum(real,-imag);
}



double &BOOM::ComplexNum::getImag()
{
  return imag;
}



double &BOOM::ComplexNum::getReal()
{
  return real;
}



double BOOM::ComplexNum::getImag() const
{
  return imag;
}



double BOOM::ComplexNum::getModulus() const
{
  return sqrt(real*real + imag*imag);
}



double BOOM::ComplexNum::getReal() const
{
  return real;
}
