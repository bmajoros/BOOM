/****************************************************************
 BetaDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_BetaDistribution_H
#define INCL_BetaDistribution_H
#include <iostream>
#include "ContinuousDistribution.H"
using namespace std;


namespace GSL {

class BetaDistribution : public GSL::ContinuousDistribution
{
public:
  BetaDistribution(double shape,double scale);
  virtual double random();
  virtual double probabilityEquals(double value);
  virtual double probabilityLessThan(double value);
  virtual double probabilityGreaterThan(double value);
  virtual double criticalValueLessThan(double P);
  virtual double criticalValueGreaterThan(double P);
private:
  double shape, scale;
};

}
#endif

