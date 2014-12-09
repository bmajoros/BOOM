/****************************************************************
 GammaDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GammaDistribution.H"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include "Random.H"
using namespace std;
using namespace GSL;


GammaDistribution::GammaDistribution(double shape,
				     double scale)
  : shape(shape), scale(scale)
{
    // ctor
}



double GammaDistribution::random() {
  return gsl_ran_gamma(Random::getGenerator(),shape,scale);
}



double GammaDistribution::probabilityEquals(double value) {
  return gsl_ran_gamma_pdf(value,shape,scale);
}



double GammaDistribution::probabilityLessThan(double value) {
  return gsl_cdf_gamma_P(value,shape,scale);
}



double GammaDistribution::probabilityGreaterThan(double value) {
  return gsl_cdf_gamma_Q(value,shape,scale);
}



double GammaDistribution::criticalValueLessThan(double P) {
  return gsl_cdf_gamma_Pinv(P,shape,scale);
}



double GammaDistribution::criticalValueGreaterThan(double P) {
  return gsl_cdf_gamma_Qinv(P,shape,scale);
}





