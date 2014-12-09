/****************************************************************
 BandedMatrix3D.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_BandedMatrix3D_H
#define INCL_BandedMatrix3D_H
#include <iostream>
#include "BandedAlignmentMatrix.H"
#include "Array1D.H"
using namespace std;
using namespace BOOM;



/****************************************************************
 A three-dimensional matrix which is banded in the first two
 dimensions.  Thus, any z-column too far from the diagonal defined
 by the (x,y) plane won't be allocated.
 ****************************************************************/

namespace BOOM {
template<class T>
class BandedMatrix3D {
public:
  BandedMatrix3D(int xDim=0,int yDim=0,int zDim=0,int bandWidth=0);
  void resize(int xDim,int yDim,int zDim,int bandWidth);
  int getFirstDim() const; // xDim
  int getSecondDim() const;// yDim
  int getThirdDim() const; // zDim
  bool getColumn(int x,int y,Array1D<T>*&); // returns false if not present
  void setAllTo(const T&);
  T &operator()(int,int,int); // throws exception if not allocated!
protected:
  BandedAlignmentMatrix< Array1D<T> > M;
  int thirdDimension;
  void allocateColumns();
};
}
using namespace BOOM;



template<class T>
BandedMatrix3D<T>::BandedMatrix3D(int xDim,int yDim,int zDim,int bandWidth)
  : M(xDim,yDim,bandWidth), thirdDimension(zDim)
{
  allocateColumns();
}



template<class T>
void BandedMatrix3D<T>::resize(int xDim,int yDim,int zDim,int bandWidth)
{
  M.resize(xDim,yDim,bandWidth);
  thirdDimension=zDim;
  allocateColumns();
}



template<class T>
void BandedMatrix3D<T>::allocateColumns()
{
  int numCells=M.numAllocatedCells();
  for(int i=0 ; i<numCells ; ++i)
    M.getIthCell(i).resize(thirdDimension);
}



template<class T>
void BandedMatrix3D<T>::setAllTo(const T &t)
{
  int numCells=M.numAllocatedCells();
  for(int i=0 ; i<numCells ; ++i) {
    Array1D<T> &column=M.getIthCell(i);
    T *p=&column[0];
    for(int j=0 ; j<thirdDimension ; ++j) *(p++)=t;
  }
}



template<class T>
int BandedMatrix3D<T>::getFirstDim() const
{
  return M.getFirstDim();
}



template<class T>
int BandedMatrix3D<T>::getSecondDim() const
{
  return M.getSecondDim();
}



template<class T>
int BandedMatrix3D<T>::getThirdDim() const
{
  return thirdDimension;
}



template<class T>
bool BandedMatrix3D<T>::getColumn(int x,int y,Array1D<T>*&col)
{
  return M.getCell(x,y,col);
}



template<class T>
T &BandedMatrix3D<T>::operator()(int x,int y,int z)
{
  Array1D<T> *col;
  if(!M.getCell(x,y,col)) throw "Column not allocated in BandedMatrix3D";
  return (*col)[z];
}


#endif
