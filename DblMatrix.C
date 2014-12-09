/***********************************************************************
 DblMatrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Stacktrace.H"
#include "GaussJordan.H"
#include "DblMatrix.H"
using namespace std;



BOOM::DblMatrix::DblMatrix(int rows,int columns) 
  : theArray(rows,columns)
{
}



BOOM::DblMatrix::DblMatrix(const BOOM::DblMatrix &m)
  : theArray(m.theArray)
{
}



ostream &operator<<(ostream &os,BOOM::DblMatrix &mat)
{
  mat.printOn(os);
  return os;
}



double BOOM::DblMatrix::operator()(int row,int col) const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  return array[row][col];
}



double &BOOM::DblMatrix::operator()(int row,int col)
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  return array[row][col];
}



BOOM::DblMatrix &BOOM::DblMatrix::operator=(BOOM::DblMatrix &other)
{
  int rows=theArray.getFirstDim();
  int cols=theArray.getSecondDim();
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<cols ; ++j)
      theArray[i][j]=other.theArray[i][j];
}



bool BOOM::DblMatrix::invert(BOOM::DblMatrix &resultingMatrix) const
{
  int rows=getNumRows(), cols=getNumColumns();
  if(cols!=rows)
    throw BOOM::Stacktrace(
      "matrix is non-square in BOOM::DblMatrix::invert()");
  if(resultingMatrix.getNumColumns()!=cols ||
     resultingMatrix.getNumRows()!=rows)
    throw BOOM::Stacktrace(
      "resulting matrix of wrong size in BOOM::DblMatrix::invert()");

  BOOM::DblMatrix &self=const_cast<BOOM::DblMatrix&>(*this);
  bool success=BOOM::GaussJordan::invert(self,resultingMatrix);
  return success;
}



int BOOM::DblMatrix::getNumColumns() const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  return array.getSecondDim();
}



int BOOM::DblMatrix::getNumRows() const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  return array.getFirstDim();
}



void BOOM::DblMatrix::addRowMultiple(int sourceRow,int destinationRow,
				   double factor)
{
  int cols=theArray.getSecondDim();
  for(int i=0 ; i<cols ; ++i)
    theArray[destinationRow][i]+=theArray[sourceRow][i]*factor;
}



void BOOM::DblMatrix::getColumn(int col,BOOM::DblArray1D &into) const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  int rows=getNumRows();
  if(into.size()!=rows) into.resize(rows);
  for(int i=0 ; i<rows ; ++i) into[i]=array[i][col];
}



void BOOM::DblMatrix::getColumn(int col,BOOM::Vector<double> &into) const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  int rows=getNumRows();
  if(into.size()!=rows) into.resize(rows);
  for(int i=0 ; i<rows ; ++i) into[i]=array[i][col];
}



void BOOM::DblMatrix::getRow(int row,BOOM::DblArray1D &into) const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  int cols=getNumColumns();
  if(into.size()!=cols) into.resize(cols);
  for(int i=0 ; i<cols ; ++i)
    into[i]=array[row][i];
}



void BOOM::DblMatrix::getRow(int row,BOOM::Vector<double> &into) const
{
  BOOM::Array2D<double> &array=const_cast<BOOM::Array2D<double>&>(theArray);
  int cols=getNumColumns();
  if(into.size()!=cols) into.resize(cols);
  for(int i=0 ; i<cols ; ++i)
    into[i]=array[row][i];
}



void BOOM::DblMatrix::multiplyRowBy(int whichRow,double factor)
{
  int cols=theArray.getSecondDim();
  for(int i=0 ; i<cols ; ++i) theArray[whichRow][i]*=factor;
}



void BOOM::DblMatrix::printOn(ostream &os)
{
  int rows=theArray.getFirstDim();
  int cols=theArray.getSecondDim();
  for(int i=0 ; i<rows ; ++i)
    {
      for(int j=0 ; j<cols ; ++j)
	os << theArray[i][j] << '\t';
      os << endl;
    }
}



void BOOM::DblMatrix::setAllTo(double to)
{
  theArray.setAllTo(to);
}



void BOOM::DblMatrix::swapRows(int r1,int r2)
{
  double tempCell;
  int cols=theArray.getSecondDim();
  for(int i=0 ; i<cols ; ++i)
    {
      tempCell=theArray[r2][i];
      theArray[r2][i]=theArray[r1][i];
      theArray[r1][i]=tempCell;
    }
}



void BOOM::DblMatrix::times(const BOOM::DblMatrix &otherMatrix,
			  BOOM::DblMatrix &resultMatrix) const
{
  BOOM::DblMatrix &thisOne=*const_cast<BOOM::DblMatrix*>(this);
  BOOM::DblMatrix &thatOne=const_cast<BOOM::DblMatrix&>(otherMatrix);
  multiply(thisOne,thatOne,resultMatrix);
}



void BOOM::DblMatrix::transpose(BOOM::DblMatrix &result) const
{
  int nRows=getNumRows(), nCols=getNumColumns();
  if(nCols!=result.getNumRows() ||
     nRows!=result.getNumColumns())
    throw BOOM::Stacktrace(
      "wrong-sized matrix in BOOM::DblMatrix::transpose");
  BOOM::Array2D<double> &thatArray=result.theArray;
  BOOM::Array2D<double> &thisArray=
    const_cast<BOOM::Array2D<double>&>(theArray);
  for(int i=0 ; i<nRows ; ++i)
    for(int j=0 ; j<nCols ; ++j)
      thatArray[j][i]=thisArray[i][j];
}



void BOOM::multiply(BOOM::DblMatrix &leftM,BOOM::DblMatrix &rightM,
		    BOOM::DblMatrix &resultMatrix)
{
  int rCols=resultMatrix.getNumColumns();
  int rRows=resultMatrix.getNumRows();
  int rowLength=leftM.getNumColumns();

  if(resultMatrix.getNumRows()!=leftM.getNumRows() ||
     resultMatrix.getNumColumns()!=rightM.getNumColumns())
    throw BOOM::Stacktrace("wrong-sized matrix in BOOM::DblMatrix.C");
  if(rightM.getNumRows()!=leftM.getNumColumns())
    throw BOOM::Stacktrace("multiplying differently-sized matrices");

  double s;
  for(int i=0 ; i<rRows ; ++i)
    for(int j=0 ; j<rCols ; ++j)
      {
	s=0.0;
	for(int k=0 ; k<rowLength ; ++k)
	  {
	    double x=rightM(k,j)*leftM(i,k);
	    s+=x;
	  }
	resultMatrix(i,j)=s;
      }
}



void BOOM::DblMatrix::resize(int r,int c)
{
  theArray.resize(r,c);
}


