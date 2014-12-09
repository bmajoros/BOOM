/***********************************************************************
 Entropy.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Entropy_H
#define INCL_BOOM_Entropy_H
#include "Vector.H"
#include <math.h>
using namespace std;

namespace BOOM {

template<class FltDbl>
class Entropy
{
public:
  static FltDbl entropy(BOOM::Vector<FltDbl> &);
  static FltDbl informationContent(BOOM::Vector<FltDbl> &);
  static FltDbl relativeEntropy(BOOM::Vector<FltDbl> &,
				BOOM::Vector<FltDbl> &);
  static FltDbl crossEntropy(BOOM::Vector<FltDbl> &,
			     BOOM::Vector<FltDbl> &);
  static FltDbl JensenShannonDivergence(BOOM::Vector<FltDbl> &,
					BOOM::Vector<FltDbl> &);
};

/*
class Entropy
{
public:
  static double entropy(BOOM::Vector<double> &);
  static double informationContent(BOOM::Vector<double> &);
  static double relativeEntropy(BOOM::Vector<double> &,
				BOOM::Vector<double> &);
  static double crossEntropy(BOOM::Vector<double> &,
			     BOOM::Vector<double> &);
};
*/



template<class FltDbl>
FltDbl BOOM::Entropy<FltDbl>::crossEntropy(BOOM::Vector<FltDbl> &P,
				   BOOM::Vector<FltDbl> &Q)
{
  /*
  cross entropy = amount of uncertainty about next symbol from data source Q
                 given that we are actually expecting the symbol to come
                 from data source P (i.e., uncertainty of Q while accounting
                 for our imperfect knowledge of Q's distribution).
   */

  FltDbl H=0.0;
  int n=P.size();
  for(int i=0 ; i<n ; ++i)
    {
      FltDbl p=P[i], q=Q[i];
      if(p>0 && q>0) H-=p*log(q);
    }
  return H;
}



template<class FltDbl>
FltDbl BOOM::Entropy<FltDbl>::entropy(BOOM::Vector<FltDbl> &P)
{
  /*
    entropy = expected number of bits needed to code data source P
              (expressed as an average per symbol)
            = amount of uncertainty about next symbol from data source
   */

  FltDbl H=0.0;
  int n=P.size();
  for(int i=0 ; i<n ; ++i)
    {
      FltDbl p=P[i];
      if(p>0) H-=p*log(p);
    }
  return H;
}



template<class FltDbl>
FltDbl BOOM::Entropy<FltDbl>::informationContent(BOOM::Vector<FltDbl> &V)
{
  int n=V.size();
  FltDbl Hmax=log(n)/log(2);
  FltDbl H=entropy(V);
  return (Hmax-H)/Hmax;
}



template<class FltDbl>
FltDbl BOOM::Entropy<FltDbl>::relativeEntropy(BOOM::Vector<FltDbl> &P,
				    BOOM::Vector<FltDbl> &Q)
{
  /*
    Rel. entropy = -sum(p*log p/q) = -sum(p log p) + sum(p log q)
                 = entropy(P) - crossEntropy(P,Q)
                 = uncertainty about P after discounting for cross entropy
                   between P and Q
   */

  FltDbl H=0.0;
  int n=P.size();
  for(int i=0 ; i<n ; ++i)
    {
      FltDbl p=P[i], q=Q[i];
      if(p>0 && q>0) H+=p*log(p/q);
    }
  return H;
}



template<class FltDbl>
FltDbl Entropy<FltDbl>::JensenShannonDivergence(BOOM::Vector<FltDbl> &P,
						BOOM::Vector<FltDbl> &Q)
{
  /* A true metric (symmetric, nonnegative, zero only when identical, and
     obeys the triangle equality), unlike KL Divergence, which is asymmetric
     and violates the triangle inequality (even in the symmetric version of
     KL Divergence).  See:
        http://en.wikipedia.org/wiki/Kullback-Leibler_divergence
  */
  
  int n=P.size();
  BOOM::Vector<FltDbl> M(n);
  for(int i=0 ; i<n ; ++i) M[i]=(P[i]+Q[i])/2.0;
  FltDbl JSD=(relativeEntropy(P,M)+relativeEntropy(Q,M))/2.0;
  return JSD;
}


}

#endif
