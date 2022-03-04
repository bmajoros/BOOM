/****************************************************************
 GslBinomial.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GslBinomial_H
#define INCL_GslBinomial_H
#include <iostream>
#include "DiscreteDistribution.H"
using namespace std;


namespace GSL {

class GslBinomial : public GSL::DiscreteDistribution
{
public:
  GslBinomial(double p);
  virtual double random(int n);
  virtual double probabilityEquals(double value,int n);
  virtual double probabilityLessThan(double value,int n);
  virtual double probabilityGreaterThan(double value,int n);
private:
  double p;
};

}
#endif

