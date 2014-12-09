/****************************************************************
 GaussianDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GaussianDistribution.H"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include "Random.H"
using namespace std;
using namespace GSL;


GaussianDistribution::GaussianDistribution(double mean,
                                           double standardDeviation)
    : mean(mean),
      SD(standardDeviation)
{
    // ctor
}



double GaussianDistribution::random() {
    return mean+gsl_ran_gaussian(Random::getGenerator(),SD);
}



double GaussianDistribution::probabilityEquals(double value) {
    return gsl_ran_gaussian_pdf(value-mean,SD);
}



double GaussianDistribution::probabilityLessThan(double value) {
    return gsl_cdf_gaussian_P(value-mean,SD);
}



double GaussianDistribution::probabilityGreaterThan(double value) {
    return gsl_cdf_gaussian_Q(value-mean,SD);
}



double GaussianDistribution::criticalValueLessThan(double P) {
    return mean+gsl_cdf_gaussian_Pinv(P,SD);
}



double GaussianDistribution::criticalValueGreaterThan(double P) {
    return mean+gsl_cdf_gaussian_Qinv(P,SD);
}





