/****************************************************************
 DirichletDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "DirichletDistribution.H"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_randist.h>
#include "Random.H"
using namespace std;
using namespace BOOM;
using namespace GSL;


DirichletDistribution::DirichletDistribution(const Array1D<double> &counts)
  : parms(counts)
{
  // ctor
}



void DirichletDistribution::generate(Array1D<double> &values)
{
  gsl_ran_dirichlet(Random::getGenerator(),parms.size(),parms.getRawArray(),
		    values.getRawArray());
}




