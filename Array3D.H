/***********************************************************************
 Array3D.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Array3D_H
#define INCL_BOOM_Array3D_H
using namespace std;
#include <iostream>
#include "String.H"
#include "Stacktrace.H"
#include "File.H"

//#define DEBUG

namespace BOOM {
template<class T>
class Array3D {
public:
template<class X>
  struct IndexedTwice {
    IndexedTwice(X *base,int rowLen);
    X &operator[](int);
    const X &operator[](int) const;
    void setAllTo(const X &);
  private: X *base; int rowLen;
  };
template<class X>
  struct IndexedOnce {
    IndexedOnce(X *base,int rowLen,int numRows);
    IndexedTwice<X> operator[](int);
    const IndexedTwice<X> operator[](int) const;
    void setAllTo(const X &);
  private: X *base; int rowLen, numRows;
  };
  Array3D(int=0,int=0,int=0);
  Array3D(const Array3D<T> &);
  virtual ~Array3D();
  T &operator()(int,int,int);
  IndexedOnce<T> operator[](int);
  const IndexedOnce<T> operator[](int) const;
  int getFirstDim() const;
  int getSecondDim() const;
  int getThirdDim() const;
  void printOn(ostream &) const;
  void readFrom(istream &);
  void setAllTo(T);
  void resize(int,int,int);
  void saveBytes(BOOM::File &); // use only for basic data types!
  void loadBytes(BOOM::File &); // use only for basic data types!
  const Array3D<T> &operator=(const Array3D<T> &);
private:
  int dim1, dim2, dim3;
  int dim2x3; // = dim2*dim3
  T *theArray;
};



/***********************************************************************
                      Some shortcut definitions
 ***********************************************************************/
typedef BOOM::Array3D<double>   DblArray3D;
typedef BOOM::Array3D<float>    FloatArray3D;
typedef BOOM::Array3D<int>      IntArray3D;
typedef BOOM::Array3D<long>     LongArray3D;
typedef BOOM::Array3D<unsigned> UnsignedArray3D;
typedef BOOM::Array3D<short>    ShortArray3D;
typedef BOOM::Array3D<char>     CharArray3D;

using namespace BOOM;



/***********************************************************************
                      IndexedTwice<X> methods
 ***********************************************************************/

template<class T>
template<class X>
BOOM::Array3D<T>::IndexedTwice<X>::IndexedTwice(X *base,int rowLen) 
  : base(base), rowLen(rowLen) 
{
  // ctor
}


template<class T>
template<class X>
X &BOOM::Array3D<T>::IndexedTwice<X>::operator[](int i) {
#ifdef DEBUG
  if(i<0 || i>=rowLen) 
    throw String("Third index out of bounds in Array3D<> : ")+i+" vs "+rowLen;
#endif
  return *(base+i);
}

template<class T>
template<class X>
const X &BOOM::Array3D<T>::IndexedTwice<X>::operator[](int i) const {
#ifdef DEBUG
  if(i<0 || i>=rowLen) 
    throw String("Third index out of bounds in Array3D<> : ")+i+" vs "+rowLen;
#endif
  return *(base+i);
}

template<class T>
template<class X>
void BOOM::Array3D<T>::IndexedTwice<X>::setAllTo(const X &x) {
  X *p=base;
  for(int i=0 ; i<rowLen ; ++i)
    *p++=x;
}



/***********************************************************************
                      IndexedOnce<X> methods
 ***********************************************************************/
template<class T>
template<class X>
Array3D<T>::IndexedOnce<X>::IndexedOnce(X *base,int rowLen,int numRows) 
  : base(base), rowLen(rowLen), numRows(numRows)
{
}

template<class T>
template<class X>
Array3D<T>::IndexedTwice<X> Array3D<T>::IndexedOnce<X>::operator[](int i) {
#ifdef DEBUG
  if(i<0 || i>=numRows) 
    throw String("Second index out of bounds in Array3D ")+i+" vs "+numRows;
#endif
  return IndexedTwice<X>(base+i*rowLen,rowLen);
}

template<class T>
template<class X>
const typename Array3D<T>::template IndexedTwice<X> Array3D<T>::IndexedOnce<X>::operator[](int 
									  i) 
  const 
{
#ifdef DEBUG
  if(i<0 || i>=numRows) 
    throw String("Second index out of bounds in Array3D ")+i+" vs "+numRows;
#endif
  return IndexedTwice<X>(base+i*rowLen,rowLen);
}

template<class T>
template<class X>
void BOOM::Array3D<T>::IndexedOnce<X>::setAllTo(const X &x)
{
  X *p=base;
  int n=rowLen*numRows;
  for(int i=0 ; i<n ; ++i)
    *p++=x;
}




/***********************************************************************
                      Array3D<X> methods
 ***********************************************************************/

template<class T>
ostream &operator<<(ostream &os,const BOOM::Array3D<T> &theArray)
{
  theArray.printOn(os);
  return os;
}



template<class T>
istream &operator>>(istream &is,BOOM::Array3D<T> &array)
{
  array.readFrom(is);
  return is;
}
}


template<class T>
BOOM::Array3D<T>::Array3D(const BOOM::Array3D<T> &other)
  : dim1(other.dim1), dim2(other.dim2), dim3(other.dim3),
    theArray(new T[other.dim1*other.dim2*other.dim3])
{
  dim2x3=dim2*dim3;
  T *dest=theArray, *src=other.theArray;
  int n=dim1*dim2*dim3;
  for(int i=0 ; i<n ; ++i) *dest++=*src++;
}



template<class T>
const BOOM::Array3D<T> &BOOM::Array3D<T>::operator=(const BOOM::Array3D<T> 
						    &other)
{
  int n;
  if(dim1!=other.dim1 || dim2!=other.dim2 || dim3!=other.dim3)
    {
      delete [] theArray;
      dim1=other.dim1;
      dim2=other.dim2;
      dim3=other.dim3;
      n=dim1*dim2*dim3;
      theArray=new T[n];
    }
  else n=dim1*dim2*dim3;
  T *dest=theArray, *src=other.theArray;
  size_t nBytes=n*sizeof(T);
  //for(int i=0 ; i<n ; ++i) *dest++=*src++;
  memcpy(dest,src,nBytes);
  dim2x3=dim2*dim3;
  return *this;
}



template<class T>
BOOM::Array3D<T>::Array3D(int d1,int d2,int d3)
  : dim1(d1), dim2(d2), dim3(d3), theArray(new T[d1*d2*d3]),
    dim2x3(dim2*dim3)
{
  // ctor
}



template<class T>
BOOM::Array3D<T>::~Array3D()
{
  // dtor
  delete [] theArray;
}



template<class T>
T &BOOM::Array3D<T>::operator()(int i,int j,int k)
{
#ifdef DEBUG
  if(i<0 || i>=dim1 || j<0 || j>=dim2 || k<0 || k>=dim3)
    throw BOOM::Stacktrace(
	BOOM::String("index out of bounds in BOOM::Array3D<>::index(")+
	i+","+j+","+k+") dim1="+dim1+" dim2="+dim2+" dim3="+dim3+")");
#endif

  int ind=i*dim2x3+j*dim3+k;
  return theArray[ind];
}



template<class T>
BOOM::Array3D<T>::IndexedOnce<T> BOOM::Array3D<T>::operator[](int i)
{
#ifdef DEBUG
  if(i>=dim1 || i<0) 
    throw String("First index out of bounds in Array3D ")+i+" vs "+dim1;
#endif
  return IndexedOnce<T>(theArray+i*dim2x3,dim3,dim2);
}



template<class T>
const typename BOOM::Array3D<T>::template IndexedOnce<T> BOOM::Array3D<T>::operator[](int i) const
{
#ifdef DEBUG
  if(i>=dim1 || i<0) 
    throw String("First index out of bounds in Array3D ")+i+" vs "+dim1;
#endif
  return IndexedOnce<T>(theArray+i*dim2x3,dim3,dim2);
}



template<class T>
int BOOM::Array3D<T>::getFirstDim() const
{
  return dim1;
}



template<class T>
int BOOM::Array3D<T>::getSecondDim() const
{
  return dim2;
}



template<class T>
int BOOM::Array3D<T>::getThirdDim() const
{
  return dim3;
}



template<class T>
void BOOM::Array3D<T>::setAllTo(T f)
{
  Array3D<T> &self=*this;
  for(int i=0 ; i<dim1 ; ++i)
    for(int j=0 ; j<dim2 ; ++j)
      for(int k=0 ; k<dim3 ; ++k)
	self(i,j,k)=f;
}



template<class T>
void BOOM::Array3D<T>::printOn(ostream &os) const
{
  Array3D<T> &self=*const_cast<BOOM::Array3D<T>*>(this);
  os<<dim1<<" "<<dim2<<" "<<dim3<<endl;
  for(int i=0 ; i<dim1 ; ++i) {
    for(int j=0 ; j<dim2 ; ++j) {
      for(int k=0 ; k<dim3 ; ++k)
	os << self(i,j,k) << '\t';
      os << endl;
    }
    os<<endl;
  }
}



template<class T>
void BOOM::Array3D<T>::readFrom(istream &is)
{
  Array3D<T> &self=*this;
  is>>dim1>>dim2>>dim3;
  resize(dim1,dim2,dim3);
  BOOM::String buf;
  for(int i=0 ; i<dim1 ; ++i)
    for(int j=0 ; j<dim2 ; ++j)
      for(int k=0 ; k<dim3 ; ++k) {
	is >> buf;
	self(i,j,k)=(T) buf;
      }  
}



template<class T>
void BOOM::Array3D<T>::resize(int x,int y,int z)
{
  delete [] theArray;
  dim1=x;
  dim2=y;
  dim3=z;
  theArray=new T[dim1*dim2*dim3];
  dim2x3=dim2*dim3;
}



template<class T>
void BOOM::Array3D<T>::saveBytes(BOOM::File &file)
{
  file << dim1 << dim2 << dim3;
  int size=sizeof(T)*dim1*dim2*dim3;
  file.write(size,static_cast<void*>(theArray));
}



template<class T>
void BOOM::Array3D<T>::loadBytes(BOOM::File &file)
{
  file >> dim1 >> dim2 >> dim3;
  resize(dim1,dim2,dim3);
  int size=sizeof(T)*dim1*dim2*dim3;
  file.read(size,static_cast<void*>(theArray));
}





#endif
