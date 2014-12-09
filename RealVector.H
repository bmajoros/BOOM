/***********************************************************************
 RealVector.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_RealVector_H
#define INCL_BOOM_RealVector_H
#include "DblMatrix.H"
using namespace std;

namespace BOOM {
class RealVector
{
public:
  RealVector(int dimension);
  double operator[](int index) const;
  double &operator[](int index);
  double distanceTo(const RealVector &other);
  double dotProduct(const RealVector &with);
  double getAngle(const RealVector &from);
  double getMagnitude();
  inline int getDimension();
  void scale(double factor);
  void unitize();
private:
  BOOM::DblMatrix theMatrix;
};
}

#endif
