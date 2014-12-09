/****************************************************************
 Matrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GSLMatrix_H
#define INCL_GSLMatrix_H
#include <iostream>
#include <gsl/gsl_eigen.h>
#include "../TNT/tnt_array2d.h"
#include <complex>
#include "../Vector.H"
#include "../String.H"
#include "Permutation.H"
using namespace std;

namespace GSL {

class Vector;

class Matrix {
public:
  Matrix(int rows=1,int cols=1); // tested
  Matrix(const GSL::Matrix &); // tested
  Matrix(const TNT::Array2D<double> &);
  Matrix(const BOOM::String &filename);
  Matrix(istream &);
  virtual ~Matrix();
  void load(istream &);
  void load(const BOOM::String &filename);
  void becomeIdentity(); // tested
  bool isSquare() const;
  double operator()(int row,int col) const;
  double &operator()(int row,int col); // tested
  const GSL::Matrix &operator=(const GSL::Matrix &);//tested
  bool invert(GSL::Matrix &resultingMatrix) const;//tested
  bool invertDiagonal(GSL::Matrix &result) const;//for diagonal matrices only
  double determinantDiag() const; // for diagonal matrices only
  double logDeterminantDiag() const; // for diagonal matrices only
  inline int getNumColumns() const;//tested
  inline int getNumRows() const;//tested
  void getColumn(int column,GSL::Vector &into) const;
  void getRow(int row,GSL::Vector &into) const;
  void setColumn(int column,const GSL::Vector &);
  void setRow(int row,const GSL::Vector &);
  void printOn(ostream &) const;//tested
  void setAllTo(double d);//tested
  void power(int power,GSL::Matrix &result);
  void transpose(); // tested
  void transpose(GSL::Matrix &) const;
  void resize(int,int);//tested
  void times(const GSL::Matrix &,GSL::Matrix &resultMatrix) const;//tested
  void multiply(const GSL::Vector &by,GSL::Vector &result) const;
  void multiplyInLogSpace(const GSL::Vector &by,GSL::Vector &result) const;
  void hadamard(const GSL::Matrix &by,GSL::Matrix &result) const;
  void divideComponentwise(const GSL::Matrix &by,GSL::Matrix &result) const;
  void sqrt();
  void convertToLogs();
  double determinant() const;//tested
  bool isZero(double epsilon=1e-15) const;//tested
  bool isIdentity(double epsilon=1e-15) const;//tested
  void getEigenVectors(GSL::Matrix &vectors, // <--columns are eigenvectors
		       GSL::Vector &values, // both are sorted, descending
		       GSL::Vector &imagParts,bool sort=true) const;
  void getEigenVectors(GSL::Matrix &vectors, // <--columns are eigenvectors
		       BOOM::Vector<complex<double> >& //sorted descending
		       eigenvalues,bool sort=true);
  static void sortEigen(GSL::Matrix &vectors,GSL::Vector &values);//descending
  static void sortEigenAbs(GSL::Matrix &vectors,GSL::Vector &values);
  int SVD(GSL::Matrix &U,GSL::Matrix &S,GSL::Matrix &V_transpose) const;
  int cholesky(GSL::Matrix &L_Lt) const;//L_Lt contains L and L_transpose
  int bilmesCholesky(GSL::Matrix &L,GSL::Matrix &diag,GSL::Matrix &Lt) const;
  int LU(GSL::Matrix &LU,GSL::Permutation &,int &sign,int &determinant) const;
  int tridiag(GSL::Matrix &U,GSL::Matrix &D,GSL::Matrix &U_transpose) const;
  //int QR(GSL::Matrix &Q,GSL::Matrix &R) const;
  void covToCor(GSL::Matrix &correlationMatrix,GSL::Vector &stdDeviations) 
    const;
  void rebuildCovMatrix(const GSL::Matrix &correlationMatrix,
			const GSL::Vector &stdDeviations);
  bool sameSize(const GSL::Matrix &) const;
  TNT::Array2D<double> *convertToTNT() const;
  GSL::Matrix &operator=(const TNT::Array2D<double> &);
  void applyThreshold(double);
  void add(const GSL::Matrix &other,GSL::Matrix &result) const;
  const GSL::Matrix &operator+=(const GSL::Matrix &other);
  void subtract(const GSL::Matrix &other,GSL::Matrix &result) const;
  void scale(double factor);
  void swapCells(int row1,int col1,int row2,int col2);
  gsl_matrix *peek() {return M;}
  void getDiagonal(GSL::Vector &diag) const;
  void findDependence(int &rowA,int &rowB,double &ratioVariance) const;
  void symmetrize(void);
  double getConditionNumber() const;
private:
  gsl_matrix *M;
};

ostream &operator<<(ostream &,const GSL::Matrix &);
istream &operator>>(istream &,GSL::Matrix &);


int Matrix::getNumColumns() const
{
  return M->size2;
}



int Matrix::getNumRows() const
{
  return M->size1;
}




}

#endif
