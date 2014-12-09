/***********************************************************************
 InversionMatrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "InversionMatrix.H"
using namespace std;



BOOM::InversionMatrix::InversionMatrix(int numRows)
  : BOOM::DblMatrix(numRows,numRows*2)
{
  setupIdentMatrix();
}



void BOOM::InversionMatrix::setupIdentMatrix()
{
  BOOM::DblMatrix &self=*this;
  int rows=getNumRows();
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<rows ; ++j)
      if(i==j) 
	self.BOOM::DblMatrix::operator()(i,j)=1.0; 
      else 
	self.BOOM::DblMatrix::operator()(i,j)=0.0;
}



void BOOM::InversionMatrix::getInverted(BOOM::DblMatrix &mat)
{
  BOOM::DblMatrix &self=*this;
  int rows=getNumRows();
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<rows ; ++j)
      mat(i,j)=self.BOOM::DblMatrix::operator()(i,j);
}



double &BOOM::InversionMatrix::operator()(int row,int column)
{
  int adjustedColumn=BOOM::DblMatrix::getNumRows()+column;
  return BOOM::DblMatrix::operator()(row,adjustedColumn);
}



bool BOOM::InversionMatrix::detectNonInvertible()
{
  BOOM::DblMatrix &self=*this;
  int rows=getNumRows();
  for(int i=0 ; i<rows ; ++i)
    {
      int j;
      for(j=0 ; j<rows ; ++j)
	if(0.0!=self(i,j)) break;
      if(rows==j) return true;
    }
  return false;
}



int BOOM::InversionMatrix::getNumColumns()
{
  return BOOM::DblMatrix::getNumColumns()/2;
}



void BOOM::InversionMatrix::install(BOOM::DblMatrix &from)
{
  BOOM::InversionMatrix &self=*this;
  int rows=getNumRows();
  for(int i=0 ; i<rows ; ++i)
    for(int j=0 ; j<rows ; ++j)
      self(i,j)=from(i,j);
}
