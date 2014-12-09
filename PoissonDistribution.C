/***********************************************************************
 PoissonDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "PoissonDistribution.H"
#include <iostream>
using namespace BOOM;


PoissonDistribution::PoissonDistribution(int lambda)
  : lambda(lambda)
{
}



double PoissonDistribution::density(int x)
{
  return density(x,lambda);
}



double PoissonDistribution::logDensity(int x)
{
  return logDensity(x,lambda);
}


