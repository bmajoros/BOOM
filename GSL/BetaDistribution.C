/****************************************************************
 BetaDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BetaDistribution.H"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include "Random.H"
using namespace std;
using namespace GSL;


BetaDistribution::BetaDistribution(double shape,
				     double scale)
  : shape(shape), scale(scale)
{
    // ctor
}



double BetaDistribution::random() {
  return gsl_ran_beta(Random::getGenerator(),shape,scale);
}



double BetaDistribution::probabilityEquals(double value) {
  return gsl_ran_beta_pdf(value,shape,scale);
}



double BetaDistribution::probabilityLessThan(double value) {
  return gsl_cdf_beta_P(value,shape,scale);
}



double BetaDistribution::probabilityGreaterThan(double value) {
  return gsl_cdf_beta_Q(value,shape,scale);
}



double BetaDistribution::criticalValueLessThan(double P) {
  return gsl_cdf_beta_Pinv(P,shape,scale);
}



double BetaDistribution::criticalValueGreaterThan(double P) {
  return gsl_cdf_beta_Qinv(P,shape,scale);
}





