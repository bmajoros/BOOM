/****************************************************************
 Interval.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Interval.H"
using namespace std;
using namespace BOOM;


Interval::Interval()
  : begin(0), end(0)
{
  // ctor
}



Interval::Interval(int begin,int end)
  : begin(begin), end(end)
{
  // ctor
}



int Interval::getBegin() const
{
  return begin;
}



int Interval::getEnd() const
{
  return end;
}



void Interval::setBegin(int b)
{
  begin=b;
}



void Interval::setEnd(int e)
{
  end=e;
}



bool Interval::overlaps(const Interval &other) const
{
  return begin<other.end && other.begin<end;
}



Interval Interval::intersect(const Interval &other) const
{
  Interval temp;
  intersect(other,temp);
  return temp;
}



void Interval::intersect(const Interval &other,Interval &into) const
{
  int b=max(begin,other.begin);
  int e=max(end,other.end);
  if(b>=e) b=e=-1;
  into.begin=b; 
  into.end=e;
}



bool Interval::isEmpty() const
{
  return begin>=end;
}



void Interval::printOn(ostream &os) const
{
  os<<"("<<begin<<","<<end<<")";
}



ostream &operator<<(ostream &os,const Interval &interval)
{
  interval.printOn(os);
  return os;
}



bool Interval::contains(int pos) const
{
  return pos>=begin && pos<end;
}



