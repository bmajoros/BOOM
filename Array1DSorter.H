/***********************************************************************
 Array1DSorter.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Array1DSorter_H
#define INCL_BOOM_Array1DSorter_H
#include <math.h>
#include "Array1D.H"
#include "Comparator.H"
#include "Random.H"
using namespace std;

/****************************************************************
  BOOM::Array1DSorter -- STANDARD QUICK-SORT ALGORITHM
*****************************************************************/
namespace BOOM {
template <class T>
class Array1DSorter
{
public:
  Array1DSorter(BOOM::Array1D<T> &,BOOM::Comparator<T> &);
  BOOM::Array1D<T> *sortAscending();
  BOOM::Array1D<T> *sortDescending();  
  BOOM::Array1D<int> *sortAscendByIndex();
  BOOM::Array1D<int> *sortDescendByIndex();
  void sortAscendInPlace();
  void sortDescendInPlace();
private:
  int numElements;
  BOOM::Array1D<T> &theVector;
  BOOM::Comparator<T> &comp;

  BOOM::Array1D<T> *quickSort();
  BOOM::Array1D<int> *sortByIndex();
  bool (BOOM::Comparator<T>::*ptrToMmbrGreater)(T &,T &);
  bool (BOOM::Comparator<T>::*ptrToMmbrLess)(T &,T &);
  int partition(BOOM::Array1D<T> &,int begin,int end);
  int partition(BOOM::Array1D<int> &,int begin,int end);
  void initIndexArray(BOOM::Array1D<int> &);
  void quickSort(BOOM::Array1D<T> &,int begin,int end);
  void quickSort(BOOM::Array1D<int> &,int begin,int end);
  void sortInPlace();
  void swap(BOOM::Array1D<T> &,int,int);
  void swap(BOOM::Array1D<int> &,int,int);
};
}


template <class T>
BOOM::Array1DSorter<T>::Array1DSorter(BOOM::Array1D<T> &v,
				      BOOM::Comparator<T> &c)
  : theVector(v), numElements(v.size()), comp(c)
{
}



template <class T>
BOOM::Array1D<T> *BOOM::Array1DSorter<T>::quickSort()
{
  BOOM::Array1D<T> &sorted=*new BOOM::Array1D<T>(numElements);
  for(int i=0 ; i<numElements ; ++i) sorted[i]=theVector[i];
  quickSort(sorted,0,numElements-1);
  return &sorted;
}



template <class T>
BOOM::Array1D<T> *BOOM::Array1DSorter<T>::sortAscending()
{
  ptrToMmbrLess=&BOOM::Comparator<T>::less;
  ptrToMmbrGreater=&BOOM::Comparator<T>::greater;
  return quickSort();
}



template <class T>
BOOM::Array1D<T> *BOOM::Array1DSorter<T>::sortDescending()
{
  ptrToMmbrLess=&BOOM::Comparator<T>::greater;
  ptrToMmbrGreater=&BOOM::Comparator<T>::less;
  return quickSort();
}



template <class T>
BOOM::Array1D<int> *BOOM::Array1DSorter<T>::sortByIndex()
{
  BOOM::Array1D<int> &indices=*new BOOM::Array1D<int>;
  initIndexArray(indices);
  quickSort(indices,0,numElements-1);
  return &indices;
}



template <class T>
BOOM::Array1D<int> *BOOM::Array1DSorter<T>::sortAscendByIndex()
{
  ptrToMmbrLess=&BOOM::Comparator<T>::less;
  ptrToMmbrGreater=&BOOM::Comparator<T>::greater;
  return sortByIndex();
}



template <class T>
BOOM::Array1D<int> *BOOM::Array1DSorter<T>::sortDescendByIndex()
{
  ptrToMmbrLess=&BOOM::Comparator<T>::greater;
  ptrToMmbrGreater=&BOOM::Comparator<T>::less;
  return sortByIndex();
}



template <class T>
int BOOM::Array1DSorter<T>::partition(BOOM::Array1D<T> &sorted,
				   int begin,int end)
{
   int pivotIndex=RandomNumber(end-begin+1)+begin;
   T temp=sorted[pivotIndex];
   swap(sorted,pivotIndex,begin);
   int i=begin-1;
   int j=end+1;
   while(true) 
     {
       do --j;
       while((comp.*ptrToMmbrGreater)(sorted[j],temp));
       do ++i;
       while((comp.*ptrToMmbrLess)(sorted[i],temp));
       if(i>=j) break;
       else swap(sorted,i,j);
     }
   return j;
}



template <class T>
int BOOM::Array1DSorter<T>::partition(BOOM::Array1D<int> &indices,
				   int begin,int end)
{
   int pivotIndex=RandomNumber(end-begin+1)+begin;
   int tempIndex=indices[pivotIndex];
   T &temp=theVector[tempIndex];
   swap(indices,pivotIndex,begin);
   int i=begin-1;
   int j=end+1;
   while(true) 
     {
       do --j;
       while((comp.*ptrToMmbrGreater)(theVector[indices[j]],temp));
       do ++i;
       while((comp.*ptrToMmbrLess)(theVector[indices[i]],temp));
       if(i>=j) break;
       else swap(indices,i,j);
     }
   return j;
}



template <class T>
void BOOM::Array1DSorter<T>::initIndexArray(BOOM::Array1D<int> &indices)
{
  indices.resize(numElements);
  for(int i=0 ; i<numElements ; ++i) indices[i]=i;
}



template <class T>
void BOOM::Array1DSorter<T>::quickSort(BOOM::Array1D<T> &sorted,
			int from,int to)
{
  if(from>=to) return;
  int pivot=partition(sorted,from,to);
  quickSort(sorted,from,pivot);
  quickSort(sorted,pivot+1,to);
}



template <class T>
void BOOM::Array1DSorter<T>::quickSort(BOOM::Array1D<int> &indices,
				       int from,int to)
{
  if(from>=to) return;
  int pivot=partition(indices,from,to);
  quickSort(indices,from,pivot);
  quickSort(indices,pivot+1,to);
}



template <class T>
void BOOM::Array1DSorter<T>::sortInPlace()
{
  quickSort(theVector,0,numElements-1);
}



template <class T>
void BOOM::Array1DSorter<T>::sortAscendInPlace()
{
  ptrToMmbrGreater=&BOOM::Comparator<T>::greater;
  ptrToMmbrLess=&BOOM::Comparator<T>::less;
  sortInPlace();
}



template <class T>
void BOOM::Array1DSorter<T>::sortDescendInPlace()
{
  ptrToMmbrGreater=&BOOM::Comparator<T>::less;
  ptrToMmbrLess=&BOOM::Comparator<T>::greater;
  return sortInPlace();
}



template <class T>
void BOOM::Array1DSorter<T>::swap(BOOM::Array1D<T> &indices,
				  int index1,int index2)
{
  T temp=indices[index1];
  indices[index1]=indices[index2];
  indices[index2]=temp;
}



template <class T>
void BOOM::Array1DSorter<T>::swap(BOOM::Array1D<int> &indices,
				  int index1,int index2)
{
  int temp=indices[index1];
  indices[index1]=indices[index2];
  indices[index2]=temp;
}


#endif
