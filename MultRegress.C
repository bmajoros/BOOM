/***********************************************************************
 MultiRegress.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "MultRegress.H"
using namespace std;



bool BOOM::MultRegress::inversionRegression(const BOOM::DblMatrix &Y,
					  const BOOM::DblMatrix &X,
					  BOOM::DblMatrix &coef)
{
  /*
    This method utilizes the fact that (due to matrix algebra):
       XA=Y  ->  A=inv(Xtr*X)*Xtr*Y
    for attribute matrix X, category matrix Y, and coefficient matrix A.

    NOTE: A better way is to do QR decomposition and avoid the matrix 
          inversion.
   */

  int numPoints=Y.getNumRows(), numPredictors=X.getNumColumns();
  if(Y.getNumColumns()!=1) 
    throw 
      "BOOM::MultRegress::regress(): Y matrix must have exactly 1 column";
  if(X.getNumRows()!=numPoints)
    throw "BOOM::MultRegress::regress(): matrices must have same # of rows";
  BOOM::DblMatrix Xtr(numPredictors,numPoints);
  X.transpose(Xtr);
  BOOM::DblMatrix XtrX(numPredictors,numPredictors);
  Xtr.times(X,XtrX);
  BOOM::DblMatrix XtrXinv(numPredictors,numPredictors);
  if(!XtrX.invert(XtrXinv)) 
    return false;
  BOOM::DblMatrix XtrXinvXtr(numPredictors,numPoints);
  XtrXinv.times(Xtr,XtrXinvXtr);
  XtrXinvXtr.times(Y,coef);
  return true;
}



bool BOOM::MultRegress::regress(const BOOM::DblMatrix &Y,
				const BOOM::DblMatrix &X,
				BOOM::DblMatrix &coef)
{
  int numColumns=X.getNumColumns(), numRows=X.getNumRows();
  BOOM::DblMatrix augmentedX(numRows,numColumns+1);
  for(int i=0 ; i<numRows ; ++i)
    {
      for(int j=0 ; j<numColumns ; ++j)
	augmentedX(i,j)=X(i,j);
      augmentedX(i,numColumns)=1.0;
    }
  bool success=inversionRegression(Y,augmentedX,coef);
  return success;
}
