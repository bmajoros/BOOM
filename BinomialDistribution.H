/***********************************************************************
 BinomialDistribution.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BinomialDistribution_H
#define INCL_BinomialDistribution_H
#include <math.h>

namespace BOOM {
class BinomialDistribution
{
public:
  static inline double likelihood(int exactNumSuccesses,int numTrials,
				  double probSuccess);
  static inline double logLik(int exactNumSuccesses,int numTrials,
			      double probSuccess);
  static double rightTailedPValue(int thisManyOrMoreSuccesses,int numTrials,
				  double probabilityOfSuccess);
};
}



//=======================================================================
//                          inline methods
//=======================================================================
double BOOM::BinomialDistribution::logLik(int x,int n,double p)
{
  // term 1: sum(d=x+1 to n) log d
  double term1=0;
  for(int d=x+1 ; d<=n ; ++d) 
    term1+=log(float(d));
  
  // term 2: -sum(d=1 to n-x) log d
  double term2=0;
  for(int d=1 ; d<=n-x ; ++d) 
    term2-=log(float(d));
  
  // term 3: x log p
  double term3=x*log(p);
  
  // term 4: (n-x) log (1-p)
  double term4=(n-x)*log(1-p);

  return term1+term2+term3+term4;
}



double BOOM::BinomialDistribution::likelihood(int x,int n,double p)
{
  return exp(logLik(x,n,p));
}


#endif
