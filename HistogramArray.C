/****************************************************************
 HistogramArray.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <fstream>
#include <iostream>
#include "HistogramArray.H"
using namespace std;
using namespace BOOM;

static Histogram<double> dummy(0,0,0,0);

HistogramArray::HistogramArray(const String &filename)
{
  load(filename);
}



HistogramArray::HistogramArray(int numHistograms,double minValue,
			       double maxValue,int numBins,
			       double pseudocount)
{
  array.resize(numHistograms);
  for(int i=0 ; i<numHistograms ; ++i) {
    array[i]=new Histogram<double>(minValue,maxValue,numBins,pseudocount);
  }
}



HistogramArray::~HistogramArray()
{
  for(int i=0 ; i<array.size() ; ++i) delete array[i];
}



void HistogramArray::save(const String &filename)
{
  ofstream os(filename.c_str());
  const int N=array.size();
  os<<"HistogramArray"<<endl;
  os<<N<<endl;
  for(int i=0 ; i<N ; ++i) {
    os<<"Histogram #"<<(i+1)<<endl;
    array[i]->save(os);
  }
}



Histogram<double> &HistogramArray::operator[](int i)
{
  return *array[i];
}



const Histogram<double> &HistogramArray::operator[](int i) const
{
  return *array[i];
}



void HistogramArray::load(const String &filename)
{
  String line;
  ifstream is(filename.c_str());
  line.getline(is); // "HistogramArray"
  line.getline(is);
  int N=line.asInt();
  array.resize(N);
  for(int i=0 ; i<N ; ++i) {
    line.getline(is); // "Histogram"
    array[i]=new Histogram<double>(is);
  }
}



void HistogramArray::normalize()
{
  const int N=array.size();
  for(int i=0 ; i<N ; ++i)
    array[i]->normalize();
}


