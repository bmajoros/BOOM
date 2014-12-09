/***********************************************************************
 GaussJordan.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_GaussJordan_H
#define INCL_BOOM_GaussJordan_H
#include "InversionMatrix.H"
using namespace std;

namespace BOOM {
class GaussJordan
{
public:
  static bool invert(BOOM::DblMatrix &thisMatrix,BOOM::DblMatrix &intoHere);
private:
  static int getNonzeroRow(int,BOOM::InversionMatrix &);
  static void zeroOut(int,BOOM::InversionMatrix &);
};
}

#endif
