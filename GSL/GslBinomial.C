/****************************************************************
 GslBinomial.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GslBinomial.H"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include "Random.H"
using namespace std;
using namespace GSL;


GslBinomial::GslBinomial(double p)
  : p(p)
{
    // ctor
}



double GslBinomial::random(int n) {
  return gsl_ran_binomial(Random::getGenerator(),p,n);
}



double GslBinomial::probabilityEquals(double value,int n) {
  return gsl_ran_binomial_pdf(value,p,n);
}



double GslBinomial::probabilityLessThan(double value,int n) {
  return gsl_cdf_binomial_P(value,p,n);
}



double GslBinomial::probabilityGreaterThan(double value,int n) {
  return gsl_cdf_binomial_Q(value,p,n);
}





