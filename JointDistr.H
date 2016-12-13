/***********************************************************************
 JointDistr.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_JointDistr_H
#define INCL_BOOM_JointDistr_H
#include "Array2D.H"
#include "Array1D.H"
using namespace std;

namespace BOOM {
class JointDistr : public BOOM::Array2D<double>
{
public:
  JointDistr(int X,int Y);
  double getMarginalX(int x) const;
  double getMarginalY(int y) const;
  void computeMarginals();
private:
  BOOM::DblArray1D marginalX, marginalY;
};
}

#endif
