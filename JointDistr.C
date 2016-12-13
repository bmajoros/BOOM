/***********************************************************************
 JointDistr.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "JointDistr.H"
using namespace std;



BOOM::JointDistr::JointDistr(int X,int Y)
  : BOOM::Array2D<double>(X,Y), marginalX(X), marginalY(Y)
{
   
}



double BOOM::JointDistr::getMarginalX(int x) const
{
  return marginalX[x];
}



double BOOM::JointDistr::getMarginalY(int y) const
{
  return marginalY[y];
}



void BOOM::JointDistr::computeMarginals()
{
  marginalX.setAllTo(0.0);
  marginalY.setAllTo(0.0);

  const int X=getFirstDim();
  const int Y=getSecondDim();
  int x, y;
  for(x=0 ; x<X ; ++x)
    for(y=0 ; y<Y ; ++y)
      {
	double entry=(*this)[x][y];
	marginalX[x]+=entry;
	marginalY[Y]+=entry;
      }  
}
