/****************************************************************
 TransitionMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GSLTransitionMatrix_H
#define INCL_GSLTransitionMatrix_H
#include "Matrix.H"
#include "Vector.H"
using namespace std;
namespace GSL {

class TransitionMatrix {
  Matrix M;
public:
  TransitionMatrix(int rows,int cols);
  double &operator()(int fromState,int toState);
  const double &operator()(int fromState,int toState) const;
  bool getEquilibrium(GSL::Vector &distr) const;
};

}
#endif
