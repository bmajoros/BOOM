/****************************************************************
 PowerMean.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <math.h>
#include "Constants.H"
#include "PowerMean.H"
using namespace std;
using namespace BOOM;


float vectMax(Vector<float> &v) {
  float m=NEGATIVE_INFINITY;
  Vector<float>::iterator cur=v.begin(), end=v.end();
  for(; cur!=end ; ++cur)
    if(*cur>m) m=*cur;
  return m;
}


float PowerMean::compute_log(Vector<float> &v,float power)
{
  const float k=power;
  if(k>=100) return vectMax(v);

  int nn=v.size();
  Vector<float> x;
  for(int i=0 ; i<nn ; ++i) {
    float f=v[i];
    if(isFinite(f)) x.push_back(f);
  }
  int n=x.size();
  if(n==0) return NEGATIVE_INFINITY;
  if(n==1) return x[0];
  double sum=1.0;
  if(k==0) { // geometric mean
    sum=0;
    for(int i=0 ; i<n ; ++i) sum+=x[i];
    return sum/n;
  }
  const float x0=x[0];
  for(int i=1 ; i<n ; ++i)
    sum+=exp(k*(x[i]-x0));
  sum=log(sum);
  sum/=k;
  sum+=x0;
  sum-=log(nn)/k;
  return sum;
}


