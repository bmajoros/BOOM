/***********************************************************************
 Factorial.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Factorial_H
#define INCL_Factorial_H
#include "Vector.H"
#include <math.h>
#include <iostream>

namespace BOOM {
class Factorial
{
  BOOM::Vector<double> logFactorials;
  inline void extend(int toX);
public:
  static Factorial global;
  inline double logFactorial(int x);
  inline double stirlingsApproximation(int x);
  inline double logStirlingApprox(int x);
};
}
using namespace BOOM;


/****************************************************************
                              inlines 
 ****************************************************************/
double Factorial::logFactorial(int x)
{
  if(x>=logFactorials.size()) extend(x);
  return logFactorials[x];
}



void Factorial::extend(int toX)
{
  int oldSize=logFactorials.size();
  logFactorials.resize(toX+1);
  double prevValue=(oldSize ? logFactorials[oldSize-1] : 0);
  int x=oldSize;
  for(long double ldX=oldSize ; x<=toX ; ++x, ++ldX)
    {
      prevValue+=log(ldX==0 ? 1 : ldX);
      logFactorials[x]=prevValue;
    }
}



double Factorial::stirlingsApproximation(int x)
{
  // e^-x * x^x * sqrt(2*pi*x)
  // or just: e^-x * x^x
  
  long double ldX=x;
  return exp(-ldX)*pow(ldX,ldX);
}



double Factorial::logStirlingApprox(int x)
{
  return x*log((long double)x)-x; // neglecting the +log(sqrt(2*pi*x)) term
}

#endif
