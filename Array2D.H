/***********************************************************************
 Array2D.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Array2D_H
#define INCL_BOOM_Array2D_H
using namespace std;
#include <iostream>
#include "String.H"
#include "Stacktrace.H"
#include "File.H"

//#define DEBUG

namespace BOOM {
template<class T>
class Array2D
{
public:
template<class X>
  class RowIn2DArray
  {
  public:
    RowIn2DArray(Array2D<X> &,int);
    inline X &operator[](int);
    inline const X &operator[](int) const;
    void setAllTo(const X &);
    void printOn(ostream &) const;
    inline X *getRawData() {return theArray.getRowPointer(firstInd);}
    inline int numBytes() const {return sizeof(X)*theArray.getSecondDim();}
  private:
    int firstInd;
    Array2D<X> &theArray;
  };
  Array2D(int=0,int=0);
  Array2D(const Array2D<T> &);
  virtual ~Array2D();
  RowIn2DArray<T> operator[](int i)
  {
    return RowIn2DArray<T>(*this,i);
  }
  const RowIn2DArray<T> operator[](int i) const
  {
    return RowIn2DArray<T>(const_cast<BOOM::Array2D<T>&>(*this),i);
  }
  T &index(int,int);
  inline T&operator()(int x,int y) {return index(x,y);}
  inline int getFirstDim() const;
  inline int getSecondDim() const;
  void printOn(ostream &) const;
  void readFrom(istream &);
  void setAllTo(T);
  void resize(int,int);
  void deleteRow(int); // rows correspond to the first index
  void deleteColumn(int); // columns correspond to the second index
  void addRow(); // existing data is preserved
  void addColumn(); // existing data is preserved
  void saveBytes(BOOM::File &); // use only for basic data types!
  void loadBytes(BOOM::File &); // use only for basic data types!
  const Array2D<T> &operator=(const Array2D<T> &);
  void getColumn(int colIndex,Vector<T> &) const;
  void getRow(int rowIndex,Vector<T> &) const;
  inline T *getRowPointer(int i) {return theArray+i*dim2;}
private:
  int dim1, dim2;
  T *theArray;
};

template<class T>
class SquareArray2D : public Array2D<T> {
public:
  SquareArray2D(int size) : Array2D<T>(size,size) {}
};
}

namespace BOOM {
typedef BOOM::Array2D<double>   DblArray2D;
typedef BOOM::Array2D<float>    FloatArray2D;
typedef BOOM::Array2D<int>      IntArray2D;
typedef BOOM::Array2D<long>     LongArray2D;
typedef BOOM::Array2D<unsigned> UnsignedArray2D;
typedef BOOM::Array2D<short>    ShortArray2D;
typedef BOOM::Array2D<char>     CharArray2D;



template<class T>
ostream &operator<<(ostream &os,const BOOM::Array2D<T> &theArray)
{
  theArray.printOn(os);
  return os;
}



template<class T>
ostream &operator<<(ostream &os,typename BOOM::Array2D<T>::template 
		    RowIn2DArray<T> const &row)
{
  row.printOn(os);
  return os;
}



template<class T>
istream &operator>>(istream &is,BOOM::Array2D<T> &array)
{
  array.readFrom(is);
  return is;
}
}


using namespace BOOM;


template<class T>
BOOM::Array2D<T>::Array2D(const BOOM::Array2D<T> &other)
  : dim1(other.dim1), dim2(other.dim2), 
    theArray(new T[other.dim1*other.dim2])
{
  T *dest=theArray, *src=other.theArray;
  int n=dim1*dim2;
  for(int i=0 ; i<n ; ++i) *dest++=*src++;
}



template<class T>
const BOOM::Array2D<T> &BOOM::Array2D<T>::operator=(const BOOM::Array2D<T> 
						    &other)
{
  if(dim1!=other.dim1 || dim2!=other.dim2) {
    dim1=other.dim1;
    dim2=other.dim2;
    const int n=dim1*dim2;
    delete [] theArray;
    theArray=new T[n];
  }
  T *dest=theArray, *src=other.theArray;
  const int n=dim1*dim2;
  for(int i=0 ; i<n ; ++i) *dest++=*src++;
}



template<class T>
BOOM::Array2D<T>::Array2D(int d1,int d2)
  : dim1(d1), dim2(d2), theArray(new T[d1*d2])
{
  // ctor
}



template<class T>
BOOM::Array2D<T>::~Array2D()
{
  // dtor
  delete [] theArray;
}



template<class T>
template<class X>
X &BOOM::Array2D<T>::RowIn2DArray<X>::operator[](int secondIndex)
{
  return theArray.index(firstInd,secondIndex);
}



template<class T>
template<class X>
const X &BOOM::Array2D<T>::RowIn2DArray<X>::operator[](int secondIndex) 
  const
{
  return theArray.index(firstInd,secondIndex);
}



template<class T>
template<class X>
void BOOM::Array2D<T>::RowIn2DArray<X>::setAllTo(const X &x)
{
  // This has been optimized for speed

  int dim2=theArray.dim2;
  T *p=&theArray.index(firstInd,0);
  for(int j=0 ; j<dim2 ; ++j, ++p)
    *p=x;
}



template<class T>
template<class X>
void BOOM::Array2D<T>::RowIn2DArray<X>::printOn(ostream &os) const
{
  int n=theArray.getSecondDim();
  for(int i=0 ; i<n ; ++i)
    os<<theArray.index(firstInd,i)<<" ";
}



template<class T>
template<class X>
BOOM::Array2D<T>::RowIn2DArray<X>::RowIn2DArray(BOOM::Array2D<X> &theArray,
					      int i)
  : theArray(theArray), firstInd(i)
{
  // ctor
}



template<class T>
T &BOOM::Array2D<T>::index(int i,int j)
{
#ifdef DEBUG
  if(i<0 || i>=dim1 || j<0 || j>=dim2)
    throw BOOM::Stacktrace(
	BOOM::String("index out of bounds in BOOM::Array2D<>::index(")+
	i+","+j+") dim1="+dim1+" dim2="+dim2+")");
#endif

  int ind=i*dim2+j;
  return theArray[ind];
}



template<class T>
int BOOM::Array2D<T>::getFirstDim() const
{
  return dim1;
}



template<class T>
int BOOM::Array2D<T>::getSecondDim() const
{
  return dim2;
}



template<class T>
void BOOM::Array2D<T>::setAllTo(T f)
{
  // This has been optimized for speed

  T *p=theArray;
  int n=dim1*dim2;
  //for(int i=0 ; i<n ; ++i, ++p)
  for(int i=n ; i ; --i, ++p)
    *p=f;
}



template<class T>
void BOOM::Array2D<T>::printOn(ostream &os) const
{
  os<<dim1<<" "<<dim2<<endl;
  for(int j=0 ; j<dim2 ; ++j)
    {
      for(int i=0 ; i<dim1 ; ++i)
	os << const_cast<BOOM::Array2D<T>*>(this)->index(i,j) << '\t';
      os << endl;
    }
}



template<class T>
void BOOM::Array2D<T>::readFrom(istream &is)
{
  is>>dim1>>dim2;
  resize(dim1,dim2);
  BOOM::String buf;
  for(int j=0 ; j<dim2 ; ++j)
    for(int i=0 ; i<dim1 ; ++i) {
      is >> buf;
      index(i,j)=(T) buf;
    }
}



template<class T>
void BOOM::Array2D<T>::resize(int x,int y)
{
  delete [] theArray;
  dim1=x;
  dim2=y;
  theArray=new T[dim1*dim2];
}



template<class T>
void BOOM::Array2D<T>::saveBytes(BOOM::File &file)
{
  file << dim1 << dim2;
  int size=sizeof(T)*dim1*dim2;
  file.write(size,static_cast<void*>(theArray));
}



template<class T>
void BOOM::Array2D<T>::loadBytes(BOOM::File &file)
{
  file >> dim1 >> dim2;
  resize(dim1,dim2);
  int size=sizeof(T)*dim1*dim2;
  file.read(size,static_cast<void*>(theArray));
}



template<class T>
void BOOM::Array2D<T>::getColumn(int col,Vector<T> &v) const
{
  const BOOM::Array2D<T> &self=*this;
  int rows=getFirstDim();
  if(v.size()!=rows) v.resize(rows);
  for(int i=0 ; i<rows ; ++i)
    v[i]=self[i][col];
}



template<class T>
void BOOM::Array2D<T>::getRow(int row,Vector<T> &v) const
{
  const BOOM::Array2D<T> &self=*this;
  int cols=getSecondDim();
  if(v.size()!=cols) v.resize(cols);
  for(int i=0 ; i<cols ; ++i)
    v[i]=self[row][i];
}



// This function is not optimized.
template<class T>
void BOOM::Array2D<T>::deleteRow(int victim)
{
  const int newDim1=dim1-1;
  T *newArray=new T[newDim1*dim2];
  for(int i=0 ; i<dim1 ; ++i) {
    if(i==victim) continue;
    const int newI=i<victim ? i : i-1;
    for(int j=0 ; j<dim2 ; ++j)
      *(newArray+newI*dim2+j)=*(theArray+i*dim2+j);
  }
  delete [] theArray;
  theArray=newArray;
  dim1=newDim1;
}



// This function is not optimized.
template<class T>
void BOOM::Array2D<T>::deleteColumn(int victim)
{
  const int newDim2=dim2-1;
  T *newArray=new T[dim1*newDim2];
  for(int j=0 ; j<dim2 ; ++j) {
    if(j==victim) continue;
    const int newJ=j<victim ? j : j-1;
    for(int i=0 ; i<dim1 ; ++i)
      *(newArray+i*newDim2+newJ)=*(theArray+i*dim2+j);
  }
  delete [] theArray;
  theArray=newArray;
  dim2=newDim2;
}



// This function is not optimized.
template<class T>
void BOOM::Array2D<T>::addRow()
{
  const int newDim1=dim1+1;
  T *newArray=new T[newDim1*dim2];
  for(int i=0 ; i<dim1 ; ++i)
    for(int j=0 ; j<dim2 ; ++j)
      *(newArray+i*dim2+j)=*(theArray+i*dim2+j);
  delete [] theArray;
  theArray=newArray;
  dim1=newDim1;
}



// This function is not optimized.
template<class T>
void BOOM::Array2D<T>::addColumn()
{
  const int newDim2=dim2+1;
  T *newArray=new T[dim1*newDim2];
  for(int i=0 ; i<dim1 ; ++i)
    for(int j=0 ; j<dim2 ; ++j)
      *(newArray+i*newDim2+j)=*(theArray+i*dim2+j);
  delete [] theArray;
  theArray=newArray;
  dim2=newDim2;
}




#endif
