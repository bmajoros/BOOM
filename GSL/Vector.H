/****************************************************************
 Vector.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GSLVector_H
#define INCL_GSLVector_H
#include <iostream>
#include <gsl/gsl_vector.h>
#include "../TNT/tnt_array1d.h"
#include "BOOM/Exceptions.H"
using namespace std;


namespace GSL {

class Matrix;

class Vector
{
public:
  Vector(int dim=0);
  Vector(const GSL::Vector &);
  Vector(const GSL::Vector *) {INTERNAL_ERROR}
  Vector(TNT::Array1D<double> &);
  Vector(gsl_vector *,bool shouldDelete=false);
  Vector(const gsl_vector *);
  virtual ~Vector();
  int getDim() const;
  double &operator[](int);
  double operator[](int) const;
  void setAllTo(double);
  void add(const GSL::Vector &other,GSL::Vector &result) const;
  void accumulate(const GSL::Vector &other);
  void sub(const GSL::Vector &other,GSL::Vector &result) const;
  void scale(double factor);
  bool operator<(const GSL::Vector &) const;
  double dotProduct(const GSL::Vector &) const;
  void matrixProduct(const GSL::Vector &other,GSL::Matrix &result) const;
  GSL::Vector &operator=(const GSL::Vector &);
  GSL::Vector &operator=(const TNT::Array1D<double> &);
  bool operator==(const GSL::Vector &) const;
  double norm() const;
  bool isNull() const;
  void addConstant(double,GSL::Vector &result) const;
  gsl_vector *peek();
  const gsl_vector *peek() const;
  void printOn(ostream &) const;
  void loadFrom(istream &);
  void resize(int);
  void cut(int elementIndex);
  void applyThreshold(double);
  void convertToLogs();
private:
  gsl_vector *V;
  bool isZeroLength;
  bool shouldDelete;
};

ostream &operator<<(ostream &,const GSL::Vector &);
istream &operator>>(istream &,GSL::Vector &);
}

#endif

