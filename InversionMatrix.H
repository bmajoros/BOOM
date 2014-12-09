/***********************************************************************
 InversionMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_InversionMatrix_H
#define INCL_BOOM_InversionMatrix_H
#include "DblMatrix.H"
using namespace std;

namespace BOOM {
class InversionMatrix : public BOOM::DblMatrix
{
public:
  InversionMatrix(int rows);
  virtual double &operator()(int row,int col);
  bool detectNonInvertible();
  virtual int getNumColumns();
  void getInverted(BOOM::DblMatrix &);
  void install(BOOM::DblMatrix &);
private:
  void setupIdentMatrix();
};
}

#endif
