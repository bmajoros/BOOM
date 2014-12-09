/***********************************************************************
 SummaryStats.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <math.h>
#include <iostream>
#include "SummaryStats.H"
using namespace std;


BOOM::SummaryStats::SummaryStats(const BOOM::Vector<int> &v)
{
  compute(v);
}



BOOM::SummaryStats::SummaryStats(const BOOM::Vector<float> &v)
{
  compute(v);
}



BOOM::SummaryStats::SummaryStats(const BOOM::Vector<double> &v)
{
  compute(v);
}



double BOOM::SummaryStats::getMax()
{
  return max;
}



double BOOM::SummaryStats::getMean()
{
  return mean;
}



double BOOM::SummaryStats::getMin()
{
  return min;
}



double BOOM::SummaryStats::getStdDev()
{
  return stddev;
}



double BOOM::SummaryStats::getVar()
{
  return stddev*stddev;
}


int BOOM::SummaryStats::getN()
{
  return n;
}



double BOOM::SummaryStats::getSum()
{
  return sum;
}



void BOOM::SummaryStats::compute(const BOOM::Vector<double> &v)
{
  n=v.size();
  double sumX=0.0, sumXX=0.0;
  int i;
  for(i=0 ; i<n ; ++i)
    {
      double x=v[i];
      sumX+=x;
      sumXX+=x*x;
      if(i==0) min=max=x;
      else if(x<min) min=x;
      else if(x>max) max=x;
    }
  mean=sumX/n;
  stddev=sqrt((sumXX-sumX*sumX/n)/(n-1.0));
  sum=sumX;
}



void BOOM::SummaryStats::compute(const BOOM::Vector<float> &v)
{
  n=v.size();
  double sumX=0.0, sumXX=0.0;
  int i;
  for(i=0 ; i<n ; ++i)
    {
      double x=(double) v[i];
      sumX+=x;
      sumXX+=x*x;
      if(i==0) min=max=x;
      else if(x<min) min=x;
      else if(x>max) max=x;
    }
  mean=sumX/n;
  stddev=sqrt((sumXX-sumX*sumX/n)/(n-1.0));
  sum=sumX;
}



void BOOM::SummaryStats::compute(const BOOM::Vector<int> &v)
{
  n=v.size();
  double sumX=0.0, sumXX=0.0;
  int i;
  for(i=0 ; i<n ; ++i)
    {
      double x=(double) v[i];
      sumX+=x;
      sumXX+=x*x;
      if(i==0) min=max=x;
      else if(x<min) min=x;
      else if(x>max) max=x;
    }
  mean=sumX/n;
  stddev=sqrt((sumXX-sumX*sumX/n)/(n-1.0));
  sum=sumX;
}
