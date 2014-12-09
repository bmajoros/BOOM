/***********************************************************************
 PoissonDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_PoissonDistribution_H
#define INCL_PoissonDistribution_H
#include "Factorial.H"

namespace BOOM {
class PoissonDistribution
{
  int lambda;
public:
  PoissonDistribution(int expected);
  double density(int observed);
  double logDensity(int observed);
  static inline double density(int observed,int expected);
  static inline double logDensity(int observed,int expected);
};
}
using namespace BOOM;


//=======================================================================
//                          inline methods
//=======================================================================
double PoissonDistribution::density(int x,int lambda)
{
  return exp(logDensity(x,lambda));
}



double PoissonDistribution::logDensity(int x,int lambda)
{
  return -lambda+x*log((long double)lambda)-
    Factorial::global.logFactorial(x);
}


#endif
