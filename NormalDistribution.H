/***********************************************************************
 NormalDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_NormalDistribution_H
#define INCL_NormalDistribution_H
#include "Constants.H"
#include "ContinuousDistr.H"

namespace BOOM {
class NormalDistribution : public ContinuousDistr
{
  static const double negLogSqrtTwoPi;
  double mu, sigma;
public:
  NormalDistribution(double mu,double sigma);
  virtual double density(double x) const;
  virtual double logDensity(double x) const;
  static inline double density(double x,double mu,double sigma);
  static inline double logDensity(double x,double mu,double sigma);
};
}
using namespace BOOM;


//=======================================================================
//                          inline methods
//=======================================================================
double NormalDistribution::density(double x,double mu,double sigma)
{
  double xMinusMuOverSigma=(x-mu)/sigma;
  return 1/(sqrt(2*PI)*sigma)*exp(-0.5*xMinusMuOverSigma*xMinusMuOverSigma);
}



double NormalDistribution::logDensity(double x,double mu,double sigma)
{
  double xMinusMuOverSigma=(x-mu)/sigma;
  return negLogSqrtTwoPi-log(sigma)-xMinusMuOverSigma*xMinusMuOverSigma/2;
}


#endif
