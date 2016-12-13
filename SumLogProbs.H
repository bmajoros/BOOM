/****************************************************************
 SumLogProbs.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 For adding probabilities in log space.  Uses Kingsbury-Raynor eq.
 as suggested in (Durbin et al., 1998).
 ****************************************************************/
#ifndef INCL_SumLogProbs_H
#define INCL_SumLogProbs_H
#include "String.H"
#include "Vector.H"
#include "Array1D.H"
#include <fstream>
#include "Constants.H"

namespace BOOM {
    
template <class REAL>
REAL sumLogProbs(REAL a,REAL b);
    
template <class REAL>
REAL sumLogProbs_ordered(REAL larger,REAL smaller);
    
template <class REAL>
REAL sumLogProbs(const Vector<REAL> &);
    
template <class REAL>
REAL sumLogProbs(const Array1D<REAL> &);


double subtractLogProbs(double logA,double logB); // log(A-B)


template<class REAL>
class LogProbsSummer {
public:
  LogProbsSummer(int chunkSize);
  void add(REAL logValue);
  REAL getSum(); // in log space
protected:
  Vector<REAL> chunk;
  int n;
};

template<class REAL>
LogProbsSummer<REAL>::LogProbsSummer(int chunkSize)
  : chunk(chunkSize), n(chunkSize)
{
  // ctor
}



template<class REAL>
void LogProbsSummer<REAL>::add(REAL r)
{
  if(chunk.size()<n) chunk.push_back(r);
  else {
    REAL sum=sumLogProbs(chunk);
    chunk.clear();
    chunk.push_back(sum);
  }
}



template<class REAL>
REAL LogProbsSummer<REAL>::getSum()
{
  return sumLogProbs(chunk);
}



//====================================================================    


template <class REAL>
REAL sumLogProbs(REAL logP,REAL logQ)
{
  if(logP>logQ) return sumLogProbs_ordered(logP,logQ);
  return sumLogProbs_ordered(logQ,logP);
}



template <class REAL>
REAL sumLogProbs_ordered(REAL largerValue,REAL smallerValue)
{
  if(smallerValue==NEGATIVE_INFINITY) return largerValue;// necessary!!!!
  return largerValue+log(1+exp(smallerValue-largerValue));
}



template <class REAL>
REAL sumLogProbs(const Vector<REAL> &x) {
    int n=x.size();
    if(n==1) return x[0];
    if(n==0) return NEGATIVE_INFINITY;
    
    // Pull out the largest value
    typename Vector<REAL>::const_iterator cur=x.begin(), end=x.end();
    REAL largestValue=*cur;
    ++cur;
    for(; cur!=end ; ++cur) {
      REAL v=*cur;
      if(v>largestValue) largestValue=v;
    }

    // Handle the case of all zeros separately
    if(!isFinite(largestValue)) return NEGATIVE_INFINITY;

    // Apply the Kingsbury-Raynor formula
    REAL sum=(REAL) 0.0;
    cur=x.begin();
    for(; cur!=end ; ++cur) {
      REAL v=*cur;
      if(!isFinite(v)) continue;
      sum+=exp(v-largestValue);
    }
    return largestValue+log(sum);
}
    


template <class REAL>
REAL sumLogProbs(const Array1D<REAL> &x) {
    int n=x.size();
    if(n==1) return x[0];
    if(n==0) return NEGATIVE_INFINITY;

    // Pull out the largest value
    const REAL *p=&x[0];
    REAL largestValue=*p;
    ++p;
    for(int i=n-1 ; i ; --i, ++p) {
      REAL v=*p;
      if(v>largestValue) largestValue=v;
    }

    // Handle the case of all zeros separately
    if(!isFinite(largestValue)) return NEGATIVE_INFINITY;

    // Apply the Kingsbury-Raynor formula
    REAL sum=(REAL) 0.0;
    p=&x[0];
    for(int i=n ; i ; --i, ++p) {
      REAL v=*p;
      if(!isFinite(v)) continue;
      sum+=exp(v-largestValue);
    }
    return largestValue+log(sum);
}


}
#endif
