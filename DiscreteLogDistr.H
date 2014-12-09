/***********************************************************************
 DiscreteLogDistr.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_DiscreteLogDistr_H
#define INCL_DiscreteLogDistr_H
#include "DiscreteDistr.H"
#include "Array1D.H"

namespace BOOM {
template<class FLOAT_OR_DOUBLE>
class DiscreteLogDistr : public DiscreteDistr<FLOAT_OR_DOUBLE>
{
  virtual void initialize(const ContinuousDistr &);
public:
  DiscreteLogDistr(int);
  DiscreteLogDistr(int,const ContinuousDistr &);
};



template<class T>
DiscreteLogDistr<T>::DiscreteLogDistr(int maxValue)
  : DiscreteDistr<T>(maxValue)
{
}



template<class T>
DiscreteLogDistr<T>::DiscreteLogDistr(int maxValue,
			      const ContinuousDistr &d)
  : DiscreteDistr<T>(maxValue)
{
  initialize(d);
}



template<class T>
void DiscreteLogDistr<T>::initialize(const ContinuousDistr &d)
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
  for(int i=0 ; i<size ; ++i)
    array[i]=log(array[i]);
  Pmean=d.density(double(mean));
}
}



#endif
