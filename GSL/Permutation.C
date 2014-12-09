/****************************************************************
 Permutation.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
****************************************************************/
#include <iostream>
#include <fstream>
#include "Permutation.H"
using namespace std;
using namespace GSL;


Permutation::Permutation(int dim=0)
{
  if(dim>0) P=gsl_permutation_calloc(dim);
  else P=NULL;
}



Permutation::Permutation(const Permutation &other)
{
  const size_t dim=other.size();
  if(dim==0) P=NULL;
  else {
    P=gsl_permutation_alloc(dim);
    gsl_permutation_memcpy(P,other.P);
  }
}



Permutation::~Permutation()
{
  if(P) gsl_permutation_free(P);
}



size_t Permutation::size() const
{
  return P ? gsl_permutation_size(P) : 0;
}



const Permutation &Permutation::operator=(const Permutation &other)
{
  int dim=other.size();
  if(dim==0) {if(P) gsl_permutation_free(P); P=NULL;}
  else {
    if(size()!=dim) {
      if(P) gsl_permutation_free(P);
      P=gsl_permutation_alloc(dim);
    }
    gsl_permutation_memcpy(P,other.P);
  }
  return *this;
}



gsl_permutation *Permutation::peek()
{
  return P;
}



const gsl_permutation *Permutation::peek() const
{
  return P;
}



bool Permutation::operator++()
{
  return gsl_permutation_next(P)==GSL_SUCCESS;
}



int Permutation::operator[](int i) const
{
  return gsl_permutation_get(P,i);
}



void Permutation::printOn(ostream &os) const
{
  const Permutation &self=*this;
  const int n=size();
  for(int i=0 ; i<n ; ++i) os<<self[i]<<" ";
}



ostream &GSL::operator<<(ostream &os,const Permutation &p)
{
  p.printOn(os);
  return os;
}




