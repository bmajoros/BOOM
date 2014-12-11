/****************************************************************
 SubsetEnumerator.C
 Copyright (C)2013 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "SubsetEnumerator.H"
using namespace std;
using namespace BOOM;

SubsetEnumerator::SubsetEnumerator(int numElements)
  : iter(numElements,BinaryAlphabet::global())
{
  one=BinaryAlphabet::global().lookup('1');
}



void SubsetEnumerator::reset()
{
  iter.reset();
}



bool SubsetEnumerator::getNext(Set<int> &indices)
{
  if(iter.done()) return false;
  indices.clear();
  Sequence seq=iter.getNextSequence();
  int L=seq.getLength();
  for(int i=0 ; i<L ; ++i)
    if(seq[i]==one) indices+=i;
  return true;
}


