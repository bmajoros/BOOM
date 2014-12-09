/***********************************************************************
 GaussJordan.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "GaussJordan.H"
using namespace std;



bool BOOM::GaussJordan::invert(BOOM::DblMatrix &thisMatrix,
			       BOOM::DblMatrix &intoHere)
{
  int numRows=thisMatrix.getNumRows();
  BOOM::InversionMatrix inversionMatrix(numRows);
  inversionMatrix.install(thisMatrix);

  for(int i=0 ; i<numRows ; ++i)
    {
      int nonzeroRow=getNonzeroRow(i,inversionMatrix);
      if(nonzeroRow<0) return false;
      if(nonzeroRow!=i) 
	inversionMatrix.swapRows(nonzeroRow,i);
      double firstNonzeroEntry=inversionMatrix(i,i);
      if(firstNonzeroEntry!=1.0)
	{
	  double factor=1.0/firstNonzeroEntry;
	  inversionMatrix.multiplyRowBy(i,factor);
	}
      zeroOut(i,inversionMatrix);
    }
  inversionMatrix.getInverted(intoHere);
  return true;
}



int BOOM::GaussJordan::getNonzeroRow(int startRow,
				     BOOM::InversionMatrix &inMatrix)
{
  int numRows=inMatrix.getNumRows();
  for(int i=startRow ; i<numRows ; ++i)
    if(inMatrix(i,startRow)!=0.0)
      return i;
  return -1;
}



void BOOM::GaussJordan::zeroOut(int exceptRow,
				BOOM::InversionMatrix &inMatrix)
{
  int numRows=inMatrix.getNumRows(); 
  for(int i=0 ; i<numRows ; ++i)
    if(i!=exceptRow)
      inMatrix.addRowMultiple(exceptRow,i,-inMatrix(i,exceptRow));
}
