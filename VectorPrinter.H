/***********************************************************************
 VectorPrinter.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_VectorPrinter_H
#define INCL_BOOM_VectorPrinter_H
#include <iostream>
#include "Vector.H"
using namespace std;
namespace BOOM {

template<class T>
ostream &operator<<(ostream &os,BOOM::Vector<T> &v)
{
  os << '[';
  int i, n=v.size();
  for(i=0 ; i<n ; ++i)
    os << v[i] << ',';
  os << ']';
}

template<class T>
void printVectorUsingIndex(ostream &os,BOOM::Vector<T> &v,
			   BOOM::Vector<int> &I)
{
  os << '[';
  int i, n=v.size();
  for(i=0 ; i<n ; ++i)
    os << v[I[i]] << ',';
  os << ']';
 
}
}

#endif
