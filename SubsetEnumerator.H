/****************************************************************
 SubsetEnumerator.H
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_SubsetEnumerator_H
#define INCL_SubsetEnumerator_H
#include <iostream>
#include "NthOrderStringIterator.H"
#include "BinaryAlphabet.H"
#include "Set.H"
using namespace std;
using namespace BOOM;
namespace BOOM {

class SubsetEnumerator {
  NthOrderStringIterator iter;
  Symbol one;
public:
  SubsetEnumerator(int numElements);
  bool getNext(Set<int> &indices);
  void reset();
};
}
#endif

