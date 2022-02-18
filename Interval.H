/****************************************************************
 Interval.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Interval_H
#define INCL_Interval_H
#include <iostream>
#include "Vector.H"
#include "Comparator.H"
using namespace std;
namespace BOOM {


/****************************************************************
                             Interval
 ****************************************************************/

class Interval {
public:
  Interval();
  Interval(int begin,int end);
  int getBegin() const;
  int getEnd() const;
  int length() const { return end-begin; }
  int getLength() const { return length(); }
  void setBegin(int);
  void setEnd(int);
  bool isEmpty() const;
  bool contains(int pos) const;
  bool contains(const Interval &) const;
  bool overlaps(const Interval &) const;
  bool nextTo(const Interval &) const; // no gap between intervals
  Interval intersect(const Interval &) const;
  void intersect(const Interval &other,Interval &into) const;
  void printOn(ostream &) const;
  bool operator==(const Interval &) const;
  int distanceTo(const Interval &) const;
  int distanceTo(const int pos) const;
  static int longest(const Vector<Interval> &);
  static void Union(Vector<Interval> &);
  static void coalesce(Vector<Interval> &);
protected:
  int begin, end; // zero-based, half open: (inclusive,exclusive]
};


/****************************************************************
                       IntervalComparator
 ****************************************************************/
struct IntervalComparator : public Comparator<Interval> {
  bool equal(Interval &a,Interval &b)
    { return a.getBegin()==b.getBegin() && a.getEnd()==b.getEnd(); }
  bool greater(Interval &a,Interval &b)
    { return a.getBegin()>b.getBegin() ||
	a.getBegin()==b.getBegin() && a.getEnd()>b.getEnd(); }
  bool less(Interval &a,Interval &b)
    { return a.getBegin()<b.getBegin() ||
	a.getBegin()==b.getBegin() && a.getEnd()<b.getEnd(); }
};


/****************************************************************
                           FloatInterval
 ****************************************************************/

class FloatInterval {
public:
  FloatInterval();
  FloatInterval(float begin,float end);
  float getBegin() const;
  float getEnd() const;
  float length() const { return end-begin; }
  float getLength() const { return length(); }
  void setBegin(float);
  void setEnd(float);
  bool isEmpty() const;
  bool contains(float pos) const;
  bool overlaps(const FloatInterval &) const;
  FloatInterval intersect(const FloatInterval &) const;
  void intersect(const FloatInterval &other,FloatInterval &into) const;
  void printOn(ostream &) const;
  bool operator==(const FloatInterval &) const;
protected:
  float begin, end;
};
}

ostream &operator<<(ostream &,const BOOM::Interval &);
ostream &operator<<(ostream &,const BOOM::FloatInterval &);

#endif

