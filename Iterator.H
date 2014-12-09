/***********************************************************************
 Iterator.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Iterator_H
#define INCL_BOOM_Iterator_H
using namespace std;

namespace BOOM {
template <class T>
class Iterator
{
public:
  virtual void operator++()=0;
  virtual void operator++(int)=0;
  virtual T &operator*()=0;
  virtual bool operator==(const BOOM::Iterator<T> &) const=0;
  virtual bool operator!=(const BOOM::Iterator<T> &i) const
    {return !(i==*this);}
  virtual BOOM::Iterator<T> &clone()=0;
};
}


#endif
