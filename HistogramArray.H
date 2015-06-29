/****************************************************************
 HistogramArray.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_HistogramArray_H
#define INCL_HistogramArray_H
#include <iostream>
#include "Histogram.H"
#include "String.H"
#include "Array1D.H"
using namespace std;
using namespace BOOM;


class HistogramArray {
public:
  HistogramArray(const String &filename);
  HistogramArray(int numHistograms,double minValue,double maxValue,int numBins,
		 double pseudocount=0.0);
  virtual ~HistogramArray();
  void save(const String &filename);
  Histogram<double> &operator[](int) const;
protected:
  Array1D<Histogram<double>*> array;
  void load(const String &filename);
};

#endif

