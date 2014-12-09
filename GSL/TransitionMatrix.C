/****************************************************************
 TransitionMatrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
****************************************************************/
#include <iostream>
#include "TransitionMatrix.H"
using namespace std;
using namespace GSL;

typedef int STATE;

TransitionMatrix::TransitionMatrix(int rows,int cols)
  : M(rows,cols)
{
  if(rows!=cols) throw "TransitionMatrix must be square";
}



double &TransitionMatrix::operator()(STATE fromState,STATE toState)
{
  return M(fromState,toState);
}



const double &TransitionMatrix::operator()(STATE fromState,STATE toState) const
{
  return M(fromState,toState);
}



bool TransitionMatrix::getEquilibrium(GSL::Vector &distr) const
{
  Matrix E;
  GSL::Vector realParts, imagParts;
  M.getEigenVectors(E,realParts,imagParts,true);
  const int n=M.getNumRows(); // square matrix, so m=n
  distr.resize(n);
  GSL::Matrix E_inv;

  //###
  //Matrix Mt=M;
  //Mt.transpose();
  //cout<<"transposed: "<<Mt<<endl;
  //cout<<realParts<<endl<<imagParts<<"\nvectors:\n"<<E<<endl;
  //###

  E.invert(E_inv);
  double a=E(0,0);
  for(int i=0 ; i<n ; ++i)
    distr[i]=a*E_inv(0,i);
}





