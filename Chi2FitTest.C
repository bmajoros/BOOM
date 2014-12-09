/***********************************************************************
 Chi2FitTest.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Chi2FitTest.H"
using namespace std;



BOOM::Chi2FitTest::Chi2FitTest(BOOM::Vector<int> &observedCounts,
				     BOOM::Vector<int> &expectedCounts,
				     BOOM::Chi2Table &table)
{
  performTest(observedCounts,expectedCounts,table);
}



bool BOOM::Chi2FitTest::goodFit()
{
  return fit;
}



double BOOM::Chi2FitTest::getChiSquared()
{
  return chiSquared;
}



double BOOM::Chi2FitTest::getP()
{
  return P;
}



void BOOM::Chi2FitTest::performTest(BOOM::Vector<int> &observedCounts,
				    BOOM::Vector<int> &expectedCounts,
				    BOOM::Chi2Table &table)
{
  chiSquared=0;
  int n=observedCounts.size();
  for(int i=0 ; i<n ; ++i)
    {
      int expected=expectedCounts[i];
      if(expected==0) continue;
      int observed=observedCounts[i];
      int numerator=observed-expected;
      chiSquared+=(numerator*numerator/expected);
    }
  int df=n-1;
  P=table.lookupP(df,chiSquared);
  fit=(P>=0.05);
}
