/****************************************************************
 Correlation.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Correlation_H
#define INCL_Correlation_H
#include <iostream>
#include <utility>
#include "Vector.H"
using namespace std;


namespace BOOM {

typedef pair<double,double> DblPoint;
typedef BOOM::Vector<DblPoint> DblPointVector;

class Correlation
{
public:
  Correlation(const DblPointVector &);
  Correlation(const Vector<float> &,const Vector<float> &);
  Correlation(const Vector<double> &,const Vector<double> &);
  double getR();
  bool isSignificant(double alpha);
  double getP(); // left-tailed cumulative pdf value
  static double computeR(double sumX,double sumXX,double sumY,double sumYY,
			 double sumXY,int n);
private:
  double r;
  int n;
  void computeR(double sumX,double sumXX,double sumY,double sumYY,
		double sumXY);
};

}

#endif

