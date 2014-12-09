/***********************************************************************
 Comparator.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Comparator_H
#define INCL_BOOM_Comparator_H
using namespace std;

namespace BOOM {
template <class T>
class Comparator
{
public:
  virtual bool equal(T &a,T &b)=0;
  virtual bool greater(T &a,T &b)=0;
  virtual bool less(T &a,T &b)=0;
};


template <class T>
class DirectComparator : public BOOM::Comparator<T>
{
public:
  virtual bool equal(T &a,T &b) {return a==b;}
  virtual bool greater(T &a,T &b) {return a>b;}
  virtual bool less(T &a,T &b) {return a<b;}
};
}

#endif
