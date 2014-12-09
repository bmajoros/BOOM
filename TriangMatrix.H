/***********************************************************************
 TriangMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_TriangMatrix_H
#define INCL_BOOM_TriangMatrix_H
#include <iostream>
#include "Vector.H"
#include "Exceptions.H"
using namespace std;

namespace BOOM {
template<class T>
class TriangMatrix
{
public:
  TriangMatrix(int n);
  TriangMatrix();
  T &operator()(int i,int j); // i must be less than j! 
  T &safeIndex(int i,int j);  // i need not be less than j
  int getN() const;
  void resize(int n);
  void printOn(ostream &);
  void setAllTo(const T &);
private:
  int arraySize;
  int n;
  BOOM::Vector<T> array;
  int computeIndex(int i,int j);
};

template<class T>
ostream &operator<<(ostream &os,TriangMatrix<T> &m) {
  m.printOn(os);
  return os;
}

}
using namespace BOOM;



template<class T>
void TriangMatrix<T>::setAllTo(const T &t) {
  for(int i=0 ; i<arraySize ; ++i) array[i]=t;
}



template<class T>
void TriangMatrix<T>::printOn(ostream &os) {
  TriangMatrix<T> &self=*this;
  for(int j=1 ; j<n ; ++j) {
    for(int i=0 ; i<j ; ++i) {
      T x=self(i,j);
      os<<x<<"\t";
    }
    os<<endl;
  }
}


template<class T>
T &BOOM::TriangMatrix<T>::operator()(int i,int j)
{
  if(i>j)
    throw RootException("BOOM::TriangMatrix::operator(): i>j");
  int index=computeIndex(i,j);
  return array[index];
}



template<class T>
T &BOOM::TriangMatrix<T>::safeIndex(int i,int j)
{
  if(i>j)
    {
      int k=i;
      i=j;
      j=k;
    }
  int index=computeIndex(i,j);
  return array[index];
}



template<class T>
BOOM::TriangMatrix<T>::TriangMatrix(int N)
  : array(((N+1)*N)/2), n(N)
{
  arraySize=array.size();
}



template<class T>
void BOOM::TriangMatrix<T>::resize(int N)
{
  array.resize(((N+1)*N)/2);
  n=N;
  arraySize=array.size();
}



template<class T>
BOOM::TriangMatrix<T>::TriangMatrix()
  : n(0),
    arraySize(0)
{
}



template<class T>
int BOOM::TriangMatrix<T>::computeIndex(int i,int j)
{
  return arraySize-(n-i-1)*(n-i)/2-n+j;
}



template<class T>
int BOOM::TriangMatrix<T>::getN() const
{
  return n;
}


#endif
