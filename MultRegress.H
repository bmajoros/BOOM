/***********************************************************************
 MultRegress.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_MultRegress_H
#define INCL_BOOM_MultRegress_H
#include "DblMatrix.H"
using namespace std;

namespace BOOM {
class MultRegress
{
public:
  bool regress(const BOOM::DblMatrix &Y,const BOOM::DblMatrix &X,
	       BOOM::DblMatrix &coef);
private:
  bool inversionRegression(const BOOM::DblMatrix &Y,
			   const BOOM::DblMatrix &X,
			   BOOM::DblMatrix &coef);
};
}

#endif
