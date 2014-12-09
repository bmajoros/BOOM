/***********************************************************************
 DiscreteDistr.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_DiscreteDistr_H
#define INCL_DiscreteDistr_H
#include "ContinuousDistr.H"
#include "Array1D.H"

namespace BOOM {
template<class FLOAT_OR_DOUBLE>
class DiscreteDistr
{
public:
  DiscreteDistr(int);
  DiscreteDistr(int,const ContinuousDistr &);
  FLOAT_OR_DOUBLE &operator[](int);
  FLOAT_OR_DOUBLE getMean();
  FLOAT_OR_DOUBLE getProbOfMean();
  void computeMean();
  void normalize();
  void useLogs();
  int domainSize() {return array.size();}
protected:
  BOOM::Array1D<FLOAT_OR_DOUBLE> array;
  FLOAT_OR_DOUBLE mean, Pmean;

  virtual void initialize(const ContinuousDistr &);
};
}



template<class T>
DiscreteDistr<T>::DiscreteDistr(int maxValue)
  : array(maxValue+1)
{
}



template<class T>
DiscreteDistr<T>::DiscreteDistr(int maxValue,
				const ContinuousDistr &d)
  : array(maxValue+1)
{
  initialize(d);
}



template<class T>
void DiscreteDistr<T>::initialize(const ContinuousDistr &d)
{
  mean=0;
  T total=0;
  int size=array.size();
  for(int i=0 ; i<size ; ++i)
    {
      T p=d.density(double(i));
      array[i]=p;
      total+=p;
    }
  for(int i=0 ; i<size ; ++i)
    {
      array[i]/=total;
      mean+=array[i]*i;
    }
  Pmean=d.density(double(mean));
}



template<class T>
void DiscreteDistr<T>::normalize()
{
  T total=0;
  int size=array.size();
  for(int i=0 ; i<size ; ++i) total+=array[i];
  for(int i=0 ; i<size ; ++i) array[i]/=total;
}



template<class T>
void DiscreteDistr<T>::useLogs()
{
  int size=array.size();
  for(int i=0 ; i<size ; ++i) 
    array[i]=log(array[i]);
  Pmean=log(Pmean);
}



template<class T>
void DiscreteDistr<T>::computeMean()
{
  mean=0;
  int size=array.size();
  for(int i=0 ; i<size ; ++i) 
    mean+=i*array[i];
  Pmean=array[int(mean+0.5)];
}




template<class T>
T &DiscreteDistr<T>::operator[](int i)
{
  return array[i];
}



template<class T>
T DiscreteDistr<T>::getMean()
{
  return mean;
}



template<class T>
T DiscreteDistr<T>::getProbOfMean()
{
  return Pmean;
}




#endif
