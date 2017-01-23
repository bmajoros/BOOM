/****************************************************************
 Vector.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <math.h>
#include "Vector.H"
#include "Matrix.H"
#include "BOOM/Exceptions.H"
#include "BOOM/String.H"
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
using namespace std;
using namespace GSL;

Vector::Vector(int dim)
  : shouldDelete(true), isZeroLength(dim==0)
{
  V=isZeroLength ? NULL : gsl_vector_alloc(dim);
}



Vector::Vector(const GSL::Vector &other)
  : shouldDelete(true), isZeroLength(other.isZeroLength)
{
  V=isZeroLength ? NULL : gsl_vector_alloc(other.V->size);
  //if(!V) INTERNAL_ERROR;
  if(V) {
    if(V->size!=other.V->size) 
      throw BOOM::String("GSL::Vector() ")+V->size+" "+other.V->size;
    gsl_vector_memcpy(V,other.V);
  }
}



Vector::Vector(TNT::Array1D<double> &t)
  : shouldDelete(true), isZeroLength(t.dim()==0)
{
  int n=t.dim();
  V=isZeroLength ? NULL : gsl_vector_alloc(n);
  for(int i=0 ; i<n ; ++i)
    gsl_vector_set(V,i,t[i]);
}



Vector::Vector(gsl_vector *V,bool shouldDelete)
  : V(V), shouldDelete(shouldDelete), isZeroLength(V==NULL)
{
}



Vector::Vector(const gsl_vector *V)
  : V(const_cast<gsl_vector*>(V)),  // ### perverse
    shouldDelete(false), isZeroLength(V==NULL)
{
}



Vector::~Vector()
{
  if(shouldDelete && V)
    gsl_vector_free(V);
}



void Vector::resize(int dim)
{
  if(dim==0) {
    isZeroLength=true;
    return;
  }
  if(getDim()!=dim) {
    if(shouldDelete && V) gsl_vector_free(V);
    V=gsl_vector_alloc(dim);
    shouldDelete=true;
  }
  isZeroLength=false;
}



int Vector::getDim() const
{
  //if(!V) INTERNAL_ERROR;// ### debugging
  if(isZeroLength) return 0;
  if(!V) INTERNAL_ERROR;
  return V->size;
}



double &Vector::operator[](int i)
{
  return *gsl_vector_ptr(V,i);
}



double Vector::operator[](int i) const
{
  return gsl_vector_get(V,i);
}



void Vector::setAllTo(double d)
{
  gsl_vector_set_all(V,d);
}



void Vector::add(const GSL::Vector &other,GSL::Vector &result) const
{
  result=*this;
  if(result.getDim()!=other.getDim()) throw "GSL::Vector::add";
  gsl_vector_add(result.V,other.V);
}



void Vector::sub(const GSL::Vector &other,GSL::Vector &result) const
{
  result=*this;
  if(result.getDim()!=other.getDim()) 
    throw BOOM::String("GSL::Vector::sub : ")+this->getDim()+" "+other.getDim()+" "+result.getDim();
  gsl_vector_sub(result.V,other.V);
}



void Vector::scale(double factor)
{
  gsl_vector_scale(V,factor);
}



double Vector::norm() const
{
  return gsl_blas_dnrm2(V);
}



bool Vector::isNull() const
{
  return gsl_vector_isnull(V);
}



void Vector::addConstant(double d,GSL::Vector &result) const
{
  result=*this;
  gsl_vector_add_constant(result.V,d);
}



void Vector::matrixProduct(const GSL::Vector &other,GSL::Matrix &result) const
{
  const Vector &self=*this;
  int n=getDim();
  result.resize(n,n);
  for(int i=0 ; i<n ; ++i)
    for(int j=0 ; j<n ; ++j)
      result(i,j)=self[i]*self[j];
}



double Vector::dotProduct(const GSL::Vector &other) const
{
  double d;
  if(getDim()!=other.getDim()) throw "GSL::Vector::dotProduct";
  gsl_blas_ddot(V,other.V,&d);
  return d;
}



GSL::Vector &Vector::operator=(const GSL::Vector &other)
{
  if(getDim()!=other.getDim()) { //V->size!=other.V->size) {
    if(shouldDelete && V) gsl_vector_free(V);
    V=other.isZeroLength ? NULL : gsl_vector_alloc(other.V->size);
    shouldDelete=true;
  }
  //if(V->size!=other.V->size) throw "GSL::Vector::operator=";
  isZeroLength=other.isZeroLength;
  if(V) gsl_vector_memcpy(V,other.V);
  return *this;
}



gsl_vector *Vector::peek()
{
  return V;
}



const gsl_vector *Vector::peek() const
{
  return V;
}



void GSL::Vector::printOn(ostream &os) const
{
  if(isZeroLength) { os<<0<<endl; return; }
  int n=V->size;
  os<<n<<" ";
  for(int i=0 ; i<n ; ++i)
    os<<gsl_vector_get(V,i)<<" ";
}



ostream &GSL::operator<<(ostream &os,const GSL::Vector &v)
{
  v.printOn(os);
  return os;
}



GSL::Vector &Vector::operator=(const TNT::Array1D<double> &t)
{
  int n=t.dim();
  resize(n);
  for(int i=0 ; i<n ; ++i)
    gsl_vector_set(V,i,t[i]);
  return *this;
}



void Vector::applyThreshold(double epsilon)
{
  if(isZeroLength) return;
  Vector &self=*this;
  int n=V->size;
  for(int i=0 ; i<n ; ++i)
    if(fabs(self[i])<epsilon) self[i]=0.0;
}



void Vector::loadFrom(istream &is)
{
  Vector &self=*this;
  int n;
  is>>n;
  resize(n);
  for(int i=0 ; i<n ; ++i)
    is>>self[i];
}



istream &GSL::operator>>(istream &is,GSL::Vector &v)
{
  v.loadFrom(is);
  return is;
}



void Vector::accumulate(const GSL::Vector &other)
{
  if(!other.isZeroLength) gsl_vector_add(V,other.V);
}



bool Vector::operator==(const GSL::Vector &other) const {
    const Vector &self=*this;
    int n=getDim(), otherN=other.getDim();
    if(n!=otherN) return false;
    for(int i=0 ; i<n ; ++i)
        if(self[i]!=other[i])
            return false;
    return true;
}



bool Vector::operator<(const GSL::Vector &other) const
{
  const GSL::Vector &self=*this;
  int n=getDim(), otherN=other.getDim();
  if(n>otherN) return false;
  else if(n<otherN) return true;

  for(int i=0 ; i<n ; ++i) {
    double s=self[i], o=other[i];
    if(s<o) return true;
    if(s>o) return false;
  }
  return false;
}



void Vector::convertToLogs()
{
  GSL::Vector &self=*this;
  int n=getDim();
  for(int i=0 ; i<n ; ++i)
    self[i]=log(self[i]);
}



