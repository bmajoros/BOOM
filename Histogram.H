/***********************************************************************
 Histogram.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Histogram_H
#define INCL_Histogram_H
#include "Array1D.H"
#include <iostream>
#include <fstream>

namespace BOOM {
template<class T>
class Histogram
{
public:
  typedef double PROBABILITY;
  Histogram(T minValue,T maxValue,int numBins,float pseudocount);
  Histogram(const BOOM::String &filename);
  void addCounts(BOOM::Vector<T> &);
  void addPoint(T);
  void normalize();
  int getBinNum(T) const;
  int getBinNum_safe(T) const;
  PROBABILITY getP(T) const;
  PROBABILITY getBin(int);
  int getNumBins() const;
  T getMin() const;
  T getMax() const;
  void getBinBoundaries(int binNum,T &binMin,T &binMax) const;
  void divideBy(const Histogram<T> &other);
  void save(const BOOM::String &filename);
  void useLogs();
  double sum() const;
  double getBinSize() const;
  void printOn(ostream &) const;
  virtual T getRightTailThreshold(double tailArea) const;
  virtual T getLeftTailThreshold(double tailArea) const;
protected:
  BOOM::Array1D<double> bins;
  T minValue, maxValue, binSize;

  void load(const BOOM::String &filename);
};
}
using namespace BOOM;



template<class T>
ostream &operator<<(ostream &os,const Histogram<T> &h)
{
  h.printOn(os);
  return os;
}



template<class T>
Histogram<T>::Histogram(T minValue,T maxValue,int numBins,float pseudocount)
  : bins(numBins), 
    minValue(minValue),
    maxValue(maxValue),
    binSize((maxValue-minValue)/numBins)
{
  // ctor

  bins.setAllTo(pseudocount);
}



template<class T>
Histogram<T>::Histogram(const BOOM::String &filename)
  : bins(0)
{
  // ctor

  load(filename);
}



template<class T>
double Histogram<T>::getBinSize() const
{
  return binSize;
}



template<class T>
void Histogram<T>::addCounts(BOOM::Vector<T> &v)
{
  typename BOOM::Vector<T>::iterator cur=v.begin(), end=v.end();
  for(; cur!=end ; ++cur)
    {
      const T &t=*cur;
      int binNum=getBinNum_safe(t);
      bins[binNum]+=1.0;
    }
}



template<class T>
void Histogram<T>::addPoint(T t)
{
  int binNum=getBinNum_safe(t);
  bins[binNum]+=1.0;
}



template<class T>
void Histogram<T>::normalize()
{
  double total=0;
  int numBins=bins.size();
  for(int i=0 ; i<numBins ; ++i)
    total+=bins[i];
  for(int i=0 ; i<numBins ; ++i)
    bins[i]/=total;
}



template<class T>
int Histogram<T>::getBinNum_safe(T t) const
{
  int binNum=int((t-minValue)/binSize);
  if(binNum<0) binNum=0;
  else if(binNum>=bins.size()) binNum=bins.size()-1;
  return binNum;
}



template<class T>
int Histogram<T>::getBinNum(T t) const
{
  int binNum=int((t-minValue)/binSize);
  return binNum;
}



template<class T>
double Histogram<T>::getP(T t) const
{
  int b=getBinNum(t);
  return b>=0 && b<bins.size() ? bins[b] : 0;
}



template<class T>
int Histogram<T>::getNumBins() const
{
  return bins.size();
}



template<class T>
T Histogram<T>::getMin() const
{
  return minValue;
}



template<class T>
T Histogram<T>::getMax() const
{
  return maxValue;
}



template<class T>
void Histogram<T>::getBinBoundaries(int binNum,T &binMin,T &binMax) const
{
  binMin=minValue+binNum*binSize;
  binMax=binMin+binSize;
}



template<class T>
double Histogram<T>::getBin(int i)
{
  return bins[i];
}



template<class T>
void Histogram<T>::divideBy(const Histogram<T> &other)
{
  int n=bins.size();
  for(int i=0 ; i<n ; ++i)
    bins[i]/=other.bins[i];
}



template<class T>
void Histogram<T>::save(const BOOM::String &filename)
{
  ofstream os(filename.c_str());
  if(!os.good()) throw BOOM::String("Can't write into file: ")+filename;
  int n=bins.size();
  os<<minValue<<"\t"<<maxValue<<"\t"<<n<<"\t"<<binSize<<endl;
  T binBegin=minValue;
  for(int i=0 ; i<n ; ++i)
    {
      T binEnd=binBegin+binSize;
      os<<binBegin<<"\t"<<binEnd<<"\t"<<bins[i]<<endl;
      binBegin=binEnd;
    }
}



template<class T>
void Histogram<T>::load(const BOOM::String &filename)
{
  ifstream is(filename.c_str());
  if(!is.good()) throw BOOM::String("Can't open file: ")+filename;
  int numBins;
  is>>minValue>>maxValue>>numBins>>binSize;
  bins.resize(numBins);
  T begin, end;
  for(int i=0 ; i<numBins ; ++i)
    is>>begin>>end>>bins[i];
}



template<class T>
void Histogram<T>::useLogs()
{
  int n=bins.size();
  for(int i=0 ; i<n ; ++i)
    bins[i]=log(bins[i]);
}



template<class T>
double Histogram<T>::sum() const
{
  double sum=0;
  int n=bins.size();
  for(int i=0 ; i<n ; ++i)
    sum+=bins[i];
  return sum;
}



template<class T>
void Histogram<T>::printOn(ostream &os) const
{
  int n=getNumBins();
  for(int i=0 ; i<n ; ++i) {
    T min, max;
    getBinBoundaries(i,min,max);
    T mid=(min+max)/2.0;
    double P=getBin(i);
    os<<mid<<"\t"<<P<<endl;
  }
}



template<class T>
T Histogram<T>::getRightTailThreshold(double tailArea) const
{
  if(tailArea==0.0) return maxValue;
  double sum=0.0;
  T binMin, binMax;
  int numBins=getNumBins();
  for(int i=numBins-1 ; i>=0 ; --i) {
    double binArea=bins[i];
    double newSum=sum+binArea;
    if(newSum>=tailArea) {
      getBinBoundaries(i,binMin,binMax);
      double portion=(tailArea-sum)/binArea;
      return binMax-portion*binSize;
    }
    sum=newSum;
  }
  return minValue;
}



template<class T>
T Histogram<T>::getLeftTailThreshold(double tailArea) const
{
  if(tailArea==0.0) return minValue;
  double sum=0.0;
  T binMin, binMax;
  int numBins=getNumBins();
  for(int i=0 ; i<numBins ; ++i) {
    double binArea=bins[i];
    double newSum=sum+binArea;
    if(newSum>=tailArea) {
      getBinBoundaries(i,binMin,binMax);
      double portion=(tailArea-sum)/binArea;
      return binMin+portion*binSize;
    }
    sum=newSum;
  }
  return maxValue;
}



#endif
