/***********************************************************************
 BinomialDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "BinomialDistribution.H"
#include <iostream>


double BOOM::BinomialDistribution::rightTailedPValue(int successes,int n,
					       double p)
{
  /*
    Uses binomial distribution to compute probability of seeing
    #successes in n trials (or more successes in n trials; hence
    the summation), assuming trials are independent and identically
    distributed (iid).  Thus, this function provides the P-value for
    a right-tailed test.
   */

  double P=0;
  for(int x=successes ; x<=n ; ++x)
    P+=density(x,n,p);

  return P;
}


