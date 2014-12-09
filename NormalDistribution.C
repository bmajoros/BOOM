/***********************************************************************
 NormalDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "NormalDistribution.H"
#include <iostream>


const double NormalDistribution::negLogSqrtTwoPi=-log((long double)2*PI)/2;


NormalDistribution::NormalDistribution(double mu,double sigma)
  : mu(mu), sigma(sigma)
{
}



double NormalDistribution::density(double x) const
{
  return density(x,mu,sigma);
}



double NormalDistribution::logDensity(double x) const
{
  return logDensity(x,mu,sigma);
}


