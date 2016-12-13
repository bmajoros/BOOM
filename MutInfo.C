/***********************************************************************
 MutInfo.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include <math.h>
#include "MutInfo.H"
using namespace std;



double BOOM::MutInfo::compute(BOOM::ContingencyTbl &table)
{
  /*
    Computes mutual information between variables whose joint
    occurrence counts are tabulated in table
   */

  table.computeTotals();

  const int height=table.getSecondDim();
  const int width=table.getFirstDim();
  const double total=(double) table.getGrandTotal();

  BOOM::JointDistr dist(width,height);
  
  int x, y;
  for(x=0 ; x<width ; ++x)
    for(y=0 ; y<height ; ++y)
      dist[x][y]=table[x][y]/total;

  return compute(dist);
}



double BOOM::MutInfo::compute(const BOOM::JointDistr &dist)
{
  /*
    Computes mutual information between two variables whose
    joint distribution is represented in dist
   */

  const int X=dist.getFirstDim();
  const int Y=dist.getSecondDim();
  double mi=0.0;
  
  for(int x=0 ; x<X ; ++x)
    for(int y=0 ; y<Y ; ++y)
      {
	double P_sub_xy=dist[x][y];
	double P_sub_x=dist.getMarginalX(x);
	double P_sub_y=dist.getMarginalY(y);
	mi+=P_sub_xy * log(P_sub_xy/(P_sub_x*P_sub_y));
      }

  return mi;
}
