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



/****************************************************************
                             Interval
 ****************************************************************/


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
  int e=min(end,other.end);
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



bool Interval::contains(const Interval &other) const
{
  return begin<=other.begin && end>=other.end;
}



bool Interval::operator==(const Interval &other) const
{
  return begin==other.begin && end==other.end;
}



int Interval::distanceTo(const Interval &other) const
{
  if(begin>other.end) return begin-other.end;
  if(other.begin>end) return other.begin-end;
  return 0;
}



int Interval::longest(const Vector<Interval> &intervals)
{
  int longest=0;
  for(Vector<Interval>::const_iterator cur=intervals.begin(),
	end=intervals.end() ; cur!=end ; ++cur) {
    const int len=(*cur).length();
    if(len>longest) longest=len;
  }
  return longest;
}




/****************************************************************
                           FloatInterval
 ****************************************************************/


FloatInterval::FloatInterval()
  : begin(0.0), end(0.0)
{
  // ctor
}



FloatInterval::FloatInterval(float begin,float end)
  : begin(begin), end(end)
{
  // ctor
}



float FloatInterval::getBegin() const
{
  return begin;
}



float FloatInterval::getEnd() const
{
  return end;
}



void FloatInterval::setBegin(float b)
{
  begin=b;
}



void FloatInterval::setEnd(float e)
{
  end=e;
}



bool FloatInterval::overlaps(const FloatInterval &other) const
{
  return begin<other.end && other.begin<end;
}



FloatInterval FloatInterval::intersect(const FloatInterval &other) const
{
  FloatInterval temp;
  intersect(other,temp);
  return temp;
}



void FloatInterval::intersect(const FloatInterval &other,FloatInterval &into)
  const
{
  float b=max(begin,other.begin);
  float e=min(end,other.end);
  if(b>=e) b=e=-1;
  into.begin=b; 
  into.end=e;
}



bool FloatInterval::isEmpty() const
{
  return begin>=end;
}



void FloatInterval::printOn(ostream &os) const
{
  os<<"("<<begin<<","<<end<<")";
}



ostream &operator<<(ostream &os,const FloatInterval &interval)
{
  interval.printOn(os);
  return os;
}



bool FloatInterval::contains(float pos) const
{
  return pos>=begin && pos<end;
}



bool FloatInterval::operator==(const FloatInterval &other) const
{
  return begin==other.begin && end==other.end;
}



