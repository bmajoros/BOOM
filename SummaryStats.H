/***********************************************************************
 SummaryStats.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_SummaryStats_H
#define INCL_BOOM_SummaryStats_H
#include "Vector.H"
using namespace std;

namespace BOOM {
class SummaryStats
{
public:
  SummaryStats(const BOOM::Vector<int> &);
  SummaryStats(const BOOM::Vector<float> &);
  SummaryStats(const BOOM::Vector<double> &);
  double getMax();
  double getMean();
  double getMin();
  double getStdDev();
  double getVar();
  int getN();
  double getSum();
private:
  int n;
  double mean, stddev, min, max, sum;
  void compute(const BOOM::Vector<double> &);
  void compute(const BOOM::Vector<float> &);
  void compute(const BOOM::Vector<int> &);
};
}

#endif
