/****************************************************************
 MultiGauss.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_MultiGauss_H
#define INCL_MultiGauss_H
#include <iostream>
#include "MultivariateDistribution.H"
#include "Vector.H"
#include "Matrix.H"
using namespace std;

namespace GSL {

class MultiGauss : public MultivariateDistribution {
public:
  MultiGauss();
  MultiGauss(const MultiGauss &);
  MultiGauss(const GSL::Vector &means,const GSL::Matrix &covarianceMatrix,
	     bool diagonal=false);
  virtual void load(istream &);
  virtual void save(ostream &) const;
  virtual void printOn(ostream &) const;
  virtual MultivariateDistribution *clone() const;
  virtual void sample(GSL::Vector &) const;
  virtual double density(const GSL::Vector &) const;
  virtual double logDensity(const GSL::Vector &) const;
  double logDensity_diag(const GSL::Vector &) const;
  const GSL::Vector &getMeans() const {return means;}
  GSL::Vector &getMeans() {return means;}
  const GSL::Matrix &getCov() const {return covarianceMatrix;}
  GSL::Matrix &getCov() {return covarianceMatrix;}
  void addVariate(float mean,float variance);
  static MultiGauss *dropVariate(const MultiGauss &source,int whichVariate);
  static void dropVariate(const MultiGauss &source,int whichVariate,
			  MultiGauss &into);
  void dropVariate(int whichVariate);
protected:
  // Distribution parameters:
  GSL::Vector means;
  GSL::Matrix covarianceMatrix;
  GSL::Vector CinvDiag;//only used if cov matrix is diagonal
  bool diagonalOnly;

  // Auxiliary variables:
  GSL::Matrix cholesky, Cinv;
  double det, detFactor, logDetFactor, scalingConstant;
  void initAux();
  void initAux_diagonal();
  double D_Cinv_Dt(const GSL::Vector &x) const;
};

}
#endif

