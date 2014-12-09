/***********************************************************************
 RealVector.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <math.h>
#include <iostream>
#include "RealVector.H"
using namespace std;



BOOM::RealVector::RealVector(int dimension)
  : theMatrix(1,dimension)
{
}



double BOOM::RealVector::operator[](int i) const
{
  return theMatrix(0,i);
}



double &BOOM::RealVector::operator[](int i)
{
  return theMatrix(0,i);
}



double BOOM::RealVector::distanceTo(const BOOM::RealVector &other)
{
  int dim=theMatrix.getNumColumns();
  double sumSquares=0;
  for(int i=0 ; i<dim ; ++i)
    {
      double d=theMatrix(0,i)-other[i];
      sumSquares+=(d*d);
    }
  return sqrt(sumSquares);
}



double BOOM::RealVector::dotProduct(const BOOM::RealVector &other)
{
  double sumProducts=0;
  int columns=theMatrix.getNumColumns();
  for(int i=0 ; i<columns ; ++i)
    sumProducts+=(other.theMatrix(0,i)*theMatrix(0,i));
  return sumProducts;
}



double BOOM::RealVector::getAngle(const BOOM::RealVector &other)
{
  double dotProd=dotProduct(other);
  double norm=getMagnitude();
  double normalizedDotProd=dotProd/(other.getMagnitude()*norm);
  double theta=acos(normalizedDotProd);
  return theta;
}



double BOOM::RealVector::getMagnitude()
{
  double sumSquares=0;

  int cols=theMatrix.getNumColumns();
  for(int i=0 ; i<cols ; ++i)
    {
      double entry=theMatrix(0,i);
      sumSquares+=(entry*entry);
    }

  return sqrt(sumSquares);
}



int BOOM::RealVector::getDimension()
{
  return theMatrix.getNumColumns();
}



void BOOM::RealVector::scale(double factor)
{
  int cols=theMatrix.getNumColumns();
  for(int i=0 ; i<cols ; ++i)
    theMatrix(0,i)*=factor;
}



void BOOM::RealVector::unitize()
{
  double factor=1.0/getMagnitude();
  scale(factor);
}
