/****************************************************************
 DirichletDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_DirichletDistribution_H
#define INCL_DirichletDistribution_H
#include <iostream>
#include "ContinuousDistribution.H"
#include "BOOM/Array1D.H"
using namespace std;
using namespace BOOM;

namespace GSL {

class DirichletDistribution
{
public:
  DirichletDistribution(const Array1D<double> &counts);
  void generate(Array1D<double> &values);

  //THE FOLLOWING FUNCTIONS ARE DEPRECATED:
  virtual double random() {}
  virtual double probabilityEquals(double value) {}
  virtual double probabilityLessThan(double value) {}
  virtual double probabilityGreaterThan(double value) {}
  virtual double criticalValueLessThan(double P) {}
  virtual double criticalValueGreaterThan(double P) {}
private:
  Array1D<double> parms;
};
}

#endif

