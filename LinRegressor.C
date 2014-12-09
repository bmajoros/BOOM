/***********************************************************************
 LinearRegressor.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <math.h>
#include "String.H"
#include "Stacktrace.H"
#include "LinRegressor.H"
using namespace std;



BOOM::LinearFunc BOOM::LinRegressor::compute(double sumX,double sumXX,
					 double sumY,double sumYY,
					 double sumXY,int n)
{
  const double yBar=sumY/n;
  const double xBar=sumX/n;

  const double Sxx=sumXX-sumX*sumX/n;
  if(Sxx==0.0) throw BOOM::String("Error in BOOM::LinRegressor: Sxx is 0");

  const double Syy=sumYY-sumY*sumY/n;
  const double Sxy=sumXY-sumX*sumY/n;

  const double slope=Sxy/Sxx;
  const double intercept=yBar-xBar*slope;
   
  const double r=Sxy/sqrt(Syy*Sxx);
  const double coefDetermination=r*r;

  return BOOM::LinearFunc(slope,intercept,coefDetermination);
}



BOOM::LinearFunc BOOM::LinRegressor::regress(DblPointVector &points)
{
  int numPoints=points.size();
  if(numPoints<2) 
    throw BOOM::String("BOOM::LinRegressor::regress(): "
		     "Regression requires at least 2 points");

  double sumXX=0.0, sumYY=0.0, sumXY=0.0, sumY=0.0, sumX=0.0;

  for(int i=0 ; i<numPoints ; ++i)
    {
      DblPoint &point=points[i];
      double x=point.first;
      double y=point.second;

      sumX+=x;
      sumY+=y;
      sumXX+=x*x;
      sumYY+=y*y;
      sumXY+=x*y;
    }

  return compute(sumX,sumXX,sumY,sumYY,sumXY,numPoints);
}



BOOM::LinearFunc BOOM::LinRegressor::regress(BOOM::Vector<double> &yValues)
{
  int numPoints=yValues.size();
  if(numPoints<2) 
    throw BOOM::String("Regressing on <2 points");

  double sumX=0.0, sumY=0.0, sumXX=0.0, sumYY=0.0, sumXY=0.0;
  for(int i=0 ; i<numPoints ; ++i)
    {
      double x=(double) i;
      double y=yValues[i];

      sumX+=x;
      sumY+=y;
      sumXX+=x*x;
      sumYY+=y*y;
      sumXY+=x*y;
    }

  return compute(sumX,sumXX,sumY,sumYY,sumXY,numPoints);
}
