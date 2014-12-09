/***********************************************************************
 LinearFunc.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_LinearFunc_H
#define INCL_BOOM_LinearFunc_H
#include <iostream>
#include <fstream>
using namespace std;

namespace BOOM {
class LinearFunc {
public:
  LinearFunc(double slope,double intercept,
		 double coefOfDeterm);
  LinearFunc();
  double operator()(double) const;
  LinearFunc &operator=(const LinearFunc &);
  double getCoefDeterm() const;
  void printOn(ostream &) const;
  void readFrom(istream &);
private:
  double slope, intercept;
  double coefOfDeterm;
};

ostream &operator<<(ostream &,const LinearFunc &);
istream &operator>>(istream &,LinearFunc &);
}

#endif
