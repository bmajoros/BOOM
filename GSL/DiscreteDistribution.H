/****************************************************************
 DiscreteDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_DiscreteDistribution_H
#define INCL_DiscreteDistribution_H
#include <iostream>
using namespace std;



namespace GSL {

class DiscreteDistribution {
public:
  virtual ~DiscreteDistribution() {}
  //virtual double random()=0;
  //virtual double probabilityEquals(double value)=0;
  //virtual double probabilityLessThan(double value)=0;
  //virtual double probabilityGreaterThan(double value)=0;
};

}
#endif

