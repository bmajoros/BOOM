/***********************************************************************
 BandedAlignmentMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BandedAlignmentMatrix_H
#define INCL_BandedAlignmentMatrix_H
#include "Array1D.H"

namespace BOOM {
template<class T>
class BandedAlignmentMatrix
{
  Array1D<T> matrix;
  Array1D<int> lowestY, highestY;
  Array1D<int> xToOffset;
  Array1D<T*> xToCell;
  int xDim, yDim, bandWidth;

  void initMatrix();
public:
  BandedAlignmentMatrix(int xDim=0,int yDim=0,int bandWidth=0);
  int getFirstDim() const; // xDim
  int getSecondDim() const;// yDim
  bool getCell(int x,int y,T*&); // returns false if not present in matrix
  int getMatrixSize() const; // in bytes
  void resize(int xDim,int yDim,int bandWidth);
  int numAllocatedCells();
  T &getIthCell(int i);
  static int recommendBandWidth(int maxBytes,int xDim);
};
}
using namespace BOOM;


template<class T>
BandedAlignmentMatrix<T>::BandedAlignmentMatrix(int xDim,int yDim,
						int bandWidth)
  : xDim(xDim), 
    yDim(yDim), 
    bandWidth(bandWidth),
    highestY(xDim),
    lowestY(xDim),
    xToOffset(xDim),
    xToCell(xDim)
{
  // ctor

  initMatrix();
}



template<class T>
void BandedAlignmentMatrix<T>::resize(int xDim,int yDim,int bandWidth)
{
  this->xDim=xDim;
  this->yDim=yDim;
  this->bandWidth=bandWidth;
  highestY.resize(xDim);
  lowestY.resize(xDim);
  xToOffset.resize(xDim);
  xToCell.resize(xDim);
  initMatrix();
}



template<class T>
int BandedAlignmentMatrix<T>::getFirstDim() const
{
  return xDim;
}



template<class T>
int BandedAlignmentMatrix<T>::getSecondDim() const
{
  return yDim;
}



template<class T>
void BandedAlignmentMatrix<T>::initMatrix()
{
  if(xDim==0 || yDim==0 || bandWidth==0) {
    matrix.resize(0);
    return;
  }
  int nextOffset=0;
  float m=yDim/float(xDim);
  if(bandWidth<m) bandWidth=int(m+1);
  for(int x=0 ; x<xDim ; ++x)
    {
      int y=int(m*x);
      int beginY=y-bandWidth;
      int endY=y+bandWidth;
      if(beginY<0) beginY=0;
      if(endY>=yDim) endY=yDim;
      int columnHeight=endY-beginY+1;
      highestY[x]=endY;
      lowestY[x]=beginY;
      xToOffset[x]=nextOffset;
      nextOffset+=columnHeight;
    }
  matrix.resize(nextOffset);
  for(int x=0 ; x<xDim ; ++x)
    xToCell[x]=&matrix[xToOffset[x]];
}



template<class T>
bool BandedAlignmentMatrix<T>::getCell(int x,int y,T *&cellPtr)
{
  int beginY=lowestY[x];
  if(y<beginY) return false;
  int endY=highestY[x];
  if(y>endY) return false;
  cellPtr=xToCell[x]+(y-beginY);
  return true;
}



template<class T>
int BandedAlignmentMatrix<T>::getMatrixSize() const
{
  return matrix.size()*sizeof(T);
}



template<class T>
int BandedAlignmentMatrix<T>::recommendBandWidth(int maxBytes,
						 int xDim)
{
  return maxBytes/(2*xDim*sizeof(T));
}



template<class T>
int BandedAlignmentMatrix<T>::numAllocatedCells()
{
  return matrix.size();
}



template<class T>
T &BandedAlignmentMatrix<T>::getIthCell(int i)
{
  return matrix[i];
}



#endif
