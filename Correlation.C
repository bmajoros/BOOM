/****************************************************************
 Correlation.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <math.h>
#include "Correlation.H"
#include <gsl/gsl_cdf.h>
#include "String.H"
using namespace std;
using namespace BOOM;


Correlation::Correlation(const DblPointVector &v)
{
  n=v.size();
  double sumXX=0.0, sumYY=0.0, sumXY=0.0, sumY=0.0, sumX=0.0;

  for(int i=0 ; i<n ; ++i)
    {
      const DblPoint &point=v[i];
      double x=point.first;
      double y=point.second;

      sumX+=x;
      sumY+=y;
      sumXX+=x*x;
      sumYY+=y*y;
      sumXY+=x*y;
    }

  r=computeR(sumX,sumXX,sumY,sumYY,sumXY,n);
}



Correlation::Correlation(const Vector<float> &a,const Vector<float> &b)
{
  n=a.size();
  double sumXX=0.0, sumYY=0.0, sumXY=0.0, sumY=0.0, sumX=0.0;

  for(int i=0 ; i<n ; ++i)
    {
      double x=a[i], y=b[i];
      sumX+=x;
      sumY+=y;
      sumXX+=x*x;
      sumYY+=y*y;
      sumXY+=x*y;
    }

  r=computeR(sumX,sumXX,sumY,sumYY,sumXY,n);
}



Correlation::Correlation(const Vector<double> &a,const Vector<double> &b)
{
  n=a.size();
  double sumXX=0.0, sumYY=0.0, sumXY=0.0, sumY=0.0, sumX=0.0;

  for(int i=0 ; i<n ; ++i)
    {
      double x=a[i], y=b[i];
      sumX+=x;
      sumY+=y;
      sumXX+=x*x;
      sumYY+=y*y;
      sumXY+=x*y;
    }

  r=computeR(sumX,sumXX,sumY,sumYY,sumXY,n);
}



double Correlation::getR()
{
  return r;
}



bool Correlation::isSignificant(double alpha)
{
  return getP()<=alpha;
}



double Correlation::getP()
{
  double t=r*sqrt(double(n-2))/sqrt(double(1-r*r));
  return gsl_cdf_tdist_Q(t,n-2);
}



double Correlation::computeR(double sumX,double sumXX,double sumY,
			     double sumYY,double sumXY,int n)
{
  const double yBar=sumY/n;
  const double xBar=sumX/n;

  const double Sxx=sumXX-sumX*sumX/n;
  if(Sxx==0.0) throw String("Error in BOOM::LinRegressor: Sxx is 0");

  const double Syy=sumYY-sumY*sumY/n;
  const double Sxy=sumXY-sumX*sumY/n;

  return Sxy/sqrt(Syy*Sxx);
}


