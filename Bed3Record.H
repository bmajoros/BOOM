/****************************************************************
 Bed3Record.H
 Copyright (C)2017 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Bed3Record_H
#define INCL_Bed3Record_H
#include <iostream>
#include "Interval.H"
#include "String.H"
using namespace std;
using namespace BOOM;


class Bed3Record {
public:
  Bed3Record() {}
  Bed3Record(const String &chr,int begin,int end);
  inline Interval &getInterval() { return interval; }
  inline int getBegin() { return interval.getBegin(); }
  inline int getEnd() { return interval.getEnd(); }
  inline String &getChr() { return chr; }
private:
  String chr;
  Interval interval;
};

#endif

