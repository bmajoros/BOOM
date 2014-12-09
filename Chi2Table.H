/***********************************************************************
 Chi2Table.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Chi2Table_H
#define INCL_BOOM_Chi2Table_H
#include "String.H"
#include "Array1D.H"
#include "Array2D.H"
using namespace std;

static const char *DEFAULT_PATH="";

namespace BOOM {
class Chi2Table
{
public:
  Chi2Table(BOOM::String filename=DEFAULT_PATH);
  float lookupChi(int df,float alpha);  
  float lookupP(int df,float chiSquared);  
private:
  void loadFromStream(istream &);

  BOOM::IntArray1D DFs;
  BOOM::Array2D<float> table;
  BOOM::FloatArray1D alphas;
  unsigned alphaToIndex(float alpha);
  unsigned dfToIndex(int df);
};
}

#endif
