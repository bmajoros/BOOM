/****************************************************************
 GaussianMixture.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GaussianMixture_H
#define INCL_GaussianMixture_H
#include <iostream>
#include "BOOM/Array1D.H"
#include "MultiGauss.H"
#include "Vector.H"
#include "MultivariateDistribution.H"
using namespace std;
using namespace BOOM;

namespace GSL {

class GaussianMixture : public MultivariateDistribution {
public:
  GaussianMixture(int numComponents=0);
  GaussianMixture(const GaussianMixture &);
  virtual void save(ostream &) const;
  virtual void load(istream &);
  virtual void printOn(ostream &) const;
  virtual double density(const GSL::Vector &) const;
  virtual double logDensity(const GSL::Vector &) const;
  virtual void sample(GSL::Vector &) const;
  virtual MultivariateDistribution *clone() const;
  int getNumComponents() const;
  int mostProbableComponent(const GSL::Vector &,double &value) const;
  void resize(int numComponents);
  double getCoef(int) const;
  double getLogCoef(int) const;
  MultiGauss &getDistr(int);
  void setCoef(int,double);
  void setDistr(int,const MultiGauss &);
  void normalizeCoefs();
  void addVariate();
  void dropVariate(int whichVariate);
  void addComponent();
protected:
  Array1D<double> coef;
  Array1D<double> logCoef;
  Array1D<MultiGauss> distr;
  void zero();
  void initLogCoef();
};

}
#endif

