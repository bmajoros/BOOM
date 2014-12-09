/****************************************************************
 Permutation.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GSLPermutation_H
#define INCL_GSLPermutation_H
#include <iostream>
#include <gsl/gsl_permutation.h>
using namespace std;

namespace GSL {

class Permutation {
public:
  Permutation(int dim=0);
  Permutation(const Permutation &);
  virtual ~Permutation();
  const Permutation &operator=(const Permutation &);
  int operator[](int) const;
  bool operator++();
  gsl_permutation *peek();
  size_t size() const;
  const gsl_permutation *peek() const;
  void printOn(ostream &) const;
private:
  gsl_permutation *P;
};

ostream &operator<<(ostream &,const Permutation &);

}

#endif
