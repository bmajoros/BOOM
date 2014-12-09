/****************************************************************
 ContinuousDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_ContinuousDistribution_H
#define INCL_ContinuousDistribution_H
#include <iostream>
using namespace std;



namespace GSL {

class ContinuousDistribution {
public:
  virtual ~ContinuousDistribution() {}
  virtual double random()=0;
  virtual double probabilityEquals(double value)=0;
  virtual double probabilityLessThan(double value)=0;
  virtual double probabilityGreaterThan(double value)=0;
  virtual double criticalValueLessThan(double P)=0;
  virtual double criticalValueGreaterThan(double P)=0;
};

}
#endif

