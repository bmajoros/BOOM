/****************************************************************
 Interval.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Interval_H
#define INCL_Interval_H
#include <iostream>
using namespace std;
namespace BOOM {

class Interval {
public:
  Interval();
  Interval(int begin,int end);
  int getBegin() const;
  int getEnd() const;
  void setBegin(int);
  void setEnd(int);
  bool isEmpty() const;
  bool contains(int pos) const;
  bool overlaps(const Interval &) const;
  Interval intersect(const Interval &) const;
  void intersect(const Interval &other,Interval &into) const;
  void printOn(ostream &) const;
  bool operator==(const Interval &) const;
protected:
  int begin, end; // zero-based, half open: (inclusive,exclusive]
};
}

ostream &operator<<(ostream &,const BOOM::Interval &);

#endif

