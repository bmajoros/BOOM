/***********************************************************************
 Point.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Point_H
#define INCL_BOOM_Point_H
using namespace std;

namespace BOOM {
template<class T>
struct BOOM::Point
{
  T x, y;
  Point(T,T);
  Point() {}
};
}


template<class T>
BOOM::Point<T>::Point(T x,T y)
{
  this->x=x;
  this->y=y;
}


#endif
