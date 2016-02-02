/****************************************************************
 CombinationIterator.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "CombinationIterator.H"
using namespace std;
using namespace BOOM;

CombinationIterator::CombinationIterator()
{
  // ctor
}



void CombinationIterator::addUnit(int n)
{
  numStates.push_back(n);
}



int CombinationIterator::numUnits() const
{
  return numStates.size();
}



void CombinationIterator::reset()
{
  const int n=numStates.size();
  if(current.size()!=n) current.resize(n);
  for(int i=0 ; i<n; ++i) current[i]=0;
  more=true;
}



bool CombinationIterator::hasMore() const
{
  return more;
}



void CombinationIterator::advance()
{
  const int n=current.size();
  for(int i=n-1 ; i>=0 ; --i) {
    int &c=current[i];
    ++c;
    if(c<numStates[i]) return;
    c=0;
  }                          
  more=false;              
}



void CombinationIterator::getCombination(Array1D<int> &into)
{
  int n=current.size();
  if(into.size()!=n) into.resize(n);
  for(int i=0 ; i<n ; ++i) into[i]=current[i];
}



CombinationIterator CombinationIterator::clone() const
{
  return *this;
}




