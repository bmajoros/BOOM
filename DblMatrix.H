/***********************************************************************
 DblMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_DblMatrix_H
#define INCL_BOOM_DblMatrix_H
#include <iostream>
#include "Array2D.H"
#include "Array1D.H"
#include "Vector.H"
using namespace std;

namespace BOOM {

class DblMatrix;

void multiply(BOOM::DblMatrix &leftM,BOOM::DblMatrix &rightM,
	      BOOM::DblMatrix &resultMatrix);

class DblMatrix
{
public:
  DblMatrix(int rows,int columns);
  DblMatrix(const BOOM::DblMatrix &other);
  virtual double operator()(int row,int col) const;
  virtual double &operator()(int row,int col);
  DblMatrix &operator=(BOOM::DblMatrix &);
  bool invert(BOOM::DblMatrix &resultingMatrix) const;
  virtual int getNumColumns() const;
  virtual int getNumRows() const;
  int getFirstDim() const {return getNumRows();}
  int getSecondDim() const {return getNumColumns();}
  void addRowMultiple(int sourceRow,int destinationRow,double factor);
  void getColumn(int column,BOOM::DblArray1D &into) const;
  void getColumn(int column,BOOM::Vector<double> &into) const;
  void getRow(int row,BOOM::DblArray1D &into) const;
  void getRow(int row,BOOM::Vector<double> &into) const;
  void multiplyRowBy(int whichRow,double factor);
  void printOn(ostream &);
  void setAllTo(double d);
  void swapRows(int r1,int r2);
  void times(const BOOM::DblMatrix &,BOOM::DblMatrix &resultMatrix) const;
  void transpose(BOOM::DblMatrix &resultMatrix) const;
  void resize(int,int);
private:
  BOOM::Array2D<double> theArray;
};
}

ostream &operator<<(ostream &,BOOM::DblMatrix &);

#endif
