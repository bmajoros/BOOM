/***********************************************************************
 Chi2IndepTest.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include <iostream>
#include "Chi2IndepTest.H"



BOOM::Chi2IndepTest::Chi2IndepTest(BOOM::Array2D<int> &contingencyTable,
			     BOOM::Chi2Table &distr)
{
   

  runTest(contingencyTable,distr);
}



bool BOOM::Chi2IndepTest::areIndependent()
{
  return independent;
}



double BOOM::Chi2IndepTest::getChiSquared()
{
  return chiSquared;
}



double BOOM::Chi2IndepTest::getP()
{
  return P;
}



void BOOM::Chi2IndepTest::runTest(BOOM::Array2D<int> &table,
				  BOOM::Chi2Table &distr)
{
  int n1=table.getFirstDim(), n2=table.getSecondDim();
  BOOM::IntArray1D rowSums(n1), columnSums(n2);
  rowSums.setAllTo(0);
  columnSums.setAllTo(0);
  float tableSum=0;

  for(int i=0 ; i<n1 ; ++i)
    for(int j=0 ; j<n2 ; ++j)
      {
	int x=table[i][j];
	rowSums[i]+=x;
	columnSums[j]+=x;
	tableSum+=x;
      }

  chiSquared=0;
  for(int i=0 ; i<n1 ; ++i)
    for(int j=0 ; j<n2 ; ++j)
      {
	int e=int(rowSums[i]/tableSum*columnSums[j]);
	if(e==0) continue;
	int o=table[i][j];
	int diff=o-e;
	float term=diff*diff/float(e);
	 
	chiSquared+=term;
      }
  
  int df=(n1-1)*(n2-1);
  P=distr.lookupP(df,chiSquared);

  independent=(P>0.05);
}
