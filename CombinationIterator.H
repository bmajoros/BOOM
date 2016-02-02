/****************************************************************
 CombinationIterator.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_CombinationIterator_H
#define INCL_CombinationIterator_H
#include <iostream>
using namespace std;
using namespace BOOM;


class CombinationIterator {
public:
  CombinationIterator();
  void addUnit(int numStates);
  int numUnits() const;
  void reset();
  bool hasMore() const;
  void advance();
  void getCombination(Array1D<int> &into);
  CombinationIterator clone() const;
private:
  Vector<int> numStates;
  Vector<int> current;
  bool more;
};

#endif

