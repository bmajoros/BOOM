/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */
#include <iostream>
#include <math.h>
#include "Entropy.H"
using namespace std;

/*

double BOOM::Entropy::crossEntropy(BOOM::Vector<double> &P,
				   BOOM::Vector<double> &Q)
{
  double H=0.0;

  int i, n=P.size();
  for(i=0 ; i<n ; ++i)
    {
      double p=P[i], q=Q[i];
      H-=p*log(q);
    }
  
  return H;
}



double BOOM::Entropy::entropy(BOOM::Vector<double> &P)
{
  double H=0.0;

  int i, n=P.size();
  for(i=0 ; i<n ; ++i)
    {
      double p=P[i];
      H-=p*log(p);
    }
  
  return H;
}



double BOOM::Entropy::informationContent(BOOM::Vector<double> &V)
{
  int n=V.size();
  double Hmax=log(n)/log(2);
  double H=entropy(V);
  return (Hmax-H)/Hmax;
}



double BOOM::Entropy::relativeEntropy(BOOM::Vector<double> &P,
				    BOOM::Vector<double> &Q)
{
  double H=0.0;

  int i, n=P.size();
  for(i=0 ; i<n ; ++i)
    {
      double p=P[i], q=Q[i];
      H-=p*log(p/q);
    }
  
  return H;
}


*/

