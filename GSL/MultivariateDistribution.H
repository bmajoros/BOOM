/****************************************************************
 MultivariateDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_MultivariateDistribution_H
#define INCL_MultivariateDistribution_H
#include <iostream>
#include <gsl/gsl_rng.h>
#include "BOOM/Random.H"
#include "BOOM/String.H"
#include "Vector.H"
#include "Matrix.H"
using namespace std;
using namespace BOOM;

namespace GSL {

enum MultivariateType {
  MVD_NONE,
  MVD_GAUSS
};
ostream &operator<<(ostream &,MultivariateType);


class MultivariateDistribution {
public:
  MultivariateDistribution();
  MultivariateDistribution(int dim,MultivariateType type);
  int getDim() const;
  MultivariateType getType() const;
  static MultivariateDistribution *load(const String &filename);
  void save(const String &filename) const;

  // Virtual methods:
  virtual void save(ostream &) const=0;
  virtual void load(istream &)=0;
  virtual void printOn(ostream &) const=0;
  virtual MultivariateDistribution *clone() const=0;
  virtual void sample(GSL::Vector &) const=0;
  virtual double density(const GSL::Vector &) const=0;
protected:
  int dim;
  MultivariateType type;
  gsl_rng *generator;
};

ostream &operator<<(ostream &,const MultivariateDistribution &);

}
#endif

