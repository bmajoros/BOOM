/***********************************************************************
 ExtremeValueDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "ExtremeValueDistribution.H"
#include <iostream>
using namespace BOOM;


ExtremeValueDistribution::ExtremeValueDistribution(double alpha,double beta)
  : alpha(alpha), beta(beta)
{
}



double ExtremeValueDistribution::density(double x) const
{
  return density(x,alpha,beta);
}



double ExtremeValueDistribution::logDensity(double x) const
{
  return logDensity(x,alpha,beta);
}



