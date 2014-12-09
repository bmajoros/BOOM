/***********************************************************************
 ContingencyTbl.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "ContingencyTbl.H"
using namespace std;



BOOM::ContingencyTbl::ContingencyTbl(int width,int height)
  : BOOM::Array2D<int>(width,height), 
    rowTotals(height), columnTotals(width)
{
  // ctor
}



int BOOM::ContingencyTbl::getColumnTotal(int column)
{
  return columnTotals[column];
}



int BOOM::ContingencyTbl::getGrandTotal()
{
  return grandTotal;
}



int BOOM::ContingencyTbl::getRowTotal(int row)
{
  return rowTotals[row];
}



void BOOM::ContingencyTbl::computeTotals()
{
  const int width=getFirstDim();
  const int height=getSecondDim();

  columnTotals.setAllTo(0);
  rowTotals.setAllTo(0);
  grandTotal=0;

  for(int x=0 ; x<width ; ++x)
    for(int y=0 ; y<height ; ++y)
      {
	int entry=(*this)[x][y];
	grandTotal+=entry;
	columnTotals[x]+=entry;
	rowTotals[y]+=entry;
      }
}
