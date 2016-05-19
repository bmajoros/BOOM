/***********************************************************************
 Array1D.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Array1D_H
#define INCL_BOOM_Array1D_H
using namespace std;
#include <iostream>
#include <sstream>
#include "Stacktrace.H"
#include "File.H"
#include "Vector.H"
#include "Exceptions.H"

//#define DEBUG

namespace BOOM {
template<class T>
class Array1D
{
public:
  Array1D(int size=0);
  Array1D(const BOOM::Array1D<T> &);
  Array1D(const BOOM::Vector<T> &);
  virtual ~Array1D();
  T &operator[](int);
  const T &operator[](int) const;
  int size() const;
  void printOn(ostream &os);
  void readFrom(istream &is);
  void resize(int newSize);
  void safeResize(int newSize); // retains original contents (nondestructive)
  void setAllTo(T);
  void saveBytes(BOOM::File &); // use only for basic data types!
  void loadBytes(BOOM::File &); // use only for basic data types!
  BOOM::Array1D<T> &operator=(const BOOM::Array1D<T> &);
  BOOM::Array1D<T> &operator=(const BOOM::Vector<T> &);
  T *getRawArray() {return array;}
  void cut(int index);
private:
  int n;
  T *array;
  void checkIndex(int index) const;
};
typedef BOOM::Array1D<double>   DblArray1D;
typedef BOOM::Array1D<float>    FloatArray1D;
typedef BOOM::Array1D<int>      IntArray1D;
typedef BOOM::Array1D<long>     LongArray1D;
typedef BOOM::Array1D<unsigned> UnsignedArray1D;
typedef BOOM::Array1D<short>    ShortArray1D;
typedef BOOM::Array1D<char>     CharArray1D;

template<class T>
ostream &operator<<(ostream &os,const BOOM::Array1D<T> &a);

template<class T>
istream &operator>>(istream &os,BOOM::Array1D<T> &a);
}






template<class T>
BOOM::Array1D<T>::Array1D(const BOOM::Array1D<T> &other)
  : array(new T[other.n]), n(other.n)
{
  int n=other.size();
  if(size()!=n) resize(n);
  for(int i=0 ; i<n ; ++i)
    array[i]=other.array[i];
}



template<class T>
BOOM::Array1D<T>::Array1D(const BOOM::Vector<T> &V)
  : array(new T[V.size()]), n(V.size())
{
  Array1D<T> &self=*this;
  for(int i=0 ; i<n ; ++i)
    self[i]=V[i];
}



template<class T>
BOOM::Array1D<T>::Array1D(int ni)
  : n(ni), array(ni?new T[ni]:NULL)
{
  // ctor
}



template<class T>
BOOM::Array1D<T>::~Array1D()
{
  delete [] array;
}



template<class T>
ostream &BOOM::operator<<(ostream &os,const BOOM::Array1D<T> &a)
{
  a.printOn(os);
  return os;
}



template<class T>
T &BOOM::Array1D<T>::operator[](int i)
{
#ifdef DEBUG
  checkIndex(i);
#endif
  return array[i];
}



template<class T>
const T &BOOM::Array1D<T>::operator[](int i) const
{
#ifdef DEBUG
  checkIndex(i);
#endif
  return array[i];
}



template<class T>
int BOOM::Array1D<T>::size() const
{
  return n;
}



template<class T>
void BOOM::Array1D<T>::checkIndex(int index) const
{
  if(index<0 || index>=n)
    {
      stringstream os;
      os << "Invalid index ("<<index<<" vs. "<<n<<") in BOOM::Array1D"
	 << ends;
      throw BOOM::Stacktrace(os.str());
    }
}



template<class T>
void BOOM::Array1D<T>::printOn(ostream &os)
{
  os<<n<<endl;
  int i;
  for(i=0 ; i<n ; ++i)
    os << array[i] << '\t';
}



template<class T>
void BOOM::Array1D<T>::readFrom(istream &is)
{
  is>>n;
  resize(n);
  int i;
  for(i=0 ; i<n ; ++i)
    is >> array[i];
}



template<class T>
istream &BOOM::operator>>(istream &is,BOOM::Array1D<T> &a)
{
  a.readFrom(is);
  return is;
}



template<class T>
void BOOM::Array1D<T>::resize(int newSize)
{
  delete [] array;
  array=(newSize ? new T[newSize] : NULL);
  n=newSize;
}



template<class T>
void BOOM::Array1D<T>::safeResize(int newSize)
{
  if(newSize==n) return;
  T *newArray=new T[newSize];
  int numCopy=min(n,newSize);
  for(int i=0 ; i<numCopy ; ++i) newArray[i]=array[i];
  delete [] array;
  array=newArray;
  n=newSize;
}



template<class T>
void BOOM::Array1D<T>::setAllTo(T t)
{
  int i;
  for(i=0 ; i<n ; ++i)
    array[i]=t;
}



template<class T>
void BOOM::Array1D<T>::saveBytes(BOOM::File &file)
{
  file << n;
  int size=n*sizeof(T);
  file.write(size,static_cast<void*>(array));
}



template<class T>
void BOOM::Array1D<T>::loadBytes(BOOM::File &file)
{
  file >> n;
  int size=n*sizeof(T);
  resize(n);
  file.read(size,static_cast<void*>(array));
}



template<class T>
BOOM::Array1D<T> &BOOM::Array1D<T>::operator=(const BOOM::Array1D<T> &other)
{
  if(n!=other.n)
    {
      delete [] array;
      n=other.n;
      array=new T[n];
    }
  for(int i=0 ; i<n ; ++i) 
    array[i]=other.array[i];
  return *this;
}



template<class T>
BOOM::Array1D<T> &BOOM::Array1D<T>::operator=(const BOOM::Vector<T> &V)
{
  int n=V.size();
  resize(n);
  BOOM::Array1D<T> &self=*this;
  for(int i=0 ; i<n ; ++i)
    self[i]=V[i];
  return self;
}



template<class T>
void BOOM::Array1D<T>::cut(int index)
{
  T *newArray=new T[n-1];
  for(int from=0, to=0 ; from<n ; ++from)
    if(from!=index) newArray[to++]=array[from];
  delete [] array;
  array=newArray;
  --n;
}


#endif
