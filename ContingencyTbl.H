/***********************************************************************
 ContingencyTbl.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_ContingencyTbl_H
#define INCL_BOOM_ContingencyTbl_H
#include "Array2D.H"
#include "Array1D.H"
using namespace std;

namespace BOOM {
class ContingencyTbl : public BOOM::IntArray2D
{
public:
  ContingencyTbl(int width,int height);
  int getColumnTotal(int column);
  int getGrandTotal();
  int getRowTotal(int row);
  void computeTotals();
private:
  int grandTotal;
  BOOM::Array1D<int> columnTotals, rowTotals;
};
}

#endif
