/***********************************************************************
 LinRegressor.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_LinRegressor_H
#define INCL_BOOM_LinRegressor_H
#include <utility>
#include "Vector.H"
#include "LinearFunc.H"
using namespace std;

namespace BOOM {
typedef pair<double,double> DblPoint;
typedef BOOM::Vector<DblPoint> DblPointVector;

class LinRegressor
{
public:
  LinearFunc regress(DblPointVector &);
  LinearFunc regress(BOOM::Vector<double> &);
private:
  LinearFunc compute(double sumX,double sumXX,double sumY,
			 double sumYY,double sumXY,int n);
};
}

#endif
