
/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */
#include "SumLogProbs.H"
using namespace std;
using namespace BOOM;

double subtractLogProbs(double logA,double logB)
{
  double smallerValue, largerValue;
  if(logA<logB) {smallerValue=logA; largerValue=logB;}
  else {smallerValue=logB; largerValue=logA;}
  if(!isFinite(logA) && !isFinite(logB)) return logA;
  if(smallerValue==NEGATIVE_INFINITY) return log(-exp(largerValue));

  return largerValue+log(1-exp(smallerValue-largerValue));
}


