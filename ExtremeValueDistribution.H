/***********************************************************************
 ExtremeValueDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_ExtremeValueDistribution_H
#define INCL_ExtremeValueDistribution_H
#include <iostream>
#include <math.h>
#include "ContinuousDistr.H"
using namespace std;

namespace BOOM {
class ExtremeValueDistribution : public ContinuousDistr
{
  double alpha, beta;
public:
  ExtremeValueDistribution(double alpha,double beta);
  virtual double density(double x) const;
  virtual double logDensity(double x) const;
  static inline double density(double x,double alpha,double beta);
  static inline double logDensity(double x,double alpha,double beta);
};
}
using namespace BOOM;

//=======================================================================
//                          inline methods
//=======================================================================
double ExtremeValueDistribution::density(double x,double alpha,double beta)
{
  double aMinusXOverB=(alpha-x)/beta;
  return exp(aMinusXOverB-exp(aMinusXOverB))/beta;
}



double ExtremeValueDistribution::logDensity(double x,double alpha,
					    double beta)
{
  double aMinusXOverB=(alpha-x)/beta;
  return aMinusXOverB-exp(aMinusXOverB)-log(beta);
}


#endif
