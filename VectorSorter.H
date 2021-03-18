/***********************************************************************
 VectorSorter.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_VectorSorter_H
#define INCL_BOOM_VectorSorter_H
#include <math.h>
#include <assert.h>
#include "Vector.H"
#include "Comparator.H"
#include "VectorPrinter.H"
#include "Random.H"
#include "Exceptions.H"
using namespace std;
namespace BOOM {

/****************************************************************
  BOOM::VectorSorter -- STANDARD QUICK-SORT ALGORITHM
*****************************************************************/
template <class T>
class VectorSorter
{
public:
  VectorSorter(BOOM::Vector<T> &,BOOM::Comparator<T> &);
  static void shuffle(BOOM::Vector<T> &);
  BOOM::Vector<T> *sortAscending();
  BOOM::Vector<T> *sortDescending();  
  BOOM::Vector<int> *sortAscendByIndex();
  BOOM::Vector<int> *sortDescendByIndex();
  void sortAscendByIndex(BOOM::Vector<int> &);
  void sortDescendByIndex(BOOM::Vector<int> &);
  void sortAscendInPlace();
  void sortDescendInPlace();
  void applyIndexMap(BOOM::Vector<int> &indices,BOOM::Vector<T> &from,
		     BOOM::Vector<T> &to);
  void applyIndexMap(BOOM::Vector<int> &indices,BOOM::Vector<T> &fromAndTo);
private:
  int numElements;
  BOOM::Vector<T> &theVector;
  BOOM::Comparator<T> &comp;

  BOOM::Vector<T> *quickSort();
  BOOM::Vector<int> *sortByIndex();
  void sortByIndex(BOOM::Vector<int> &into);
  bool (BOOM::Comparator<T>::*ptrToMmbrGreater)(T &,T &);
  bool (BOOM::Comparator<T>::*ptrToMmbrLess)(T &,T &);
  int partition(BOOM::Vector<T> &,int begin,int end);
  int partitionIndices(BOOM::Vector<int> &indices,BOOM::Vector<T> &,
		       int begin,int end);
  void initIndexArray(BOOM::Vector<int> &);
  void quickSort(BOOM::Vector<T> &,int begin,int end);
  void quickSortIndices(BOOM::Vector<int> &indices,BOOM::Vector<T> &,
			int begin,int end);
  void sortInPlace();
  void swap(BOOM::Vector<T> &,int,int);
  void swapIndices(BOOM::Vector<int> &,int,int);
  void setupAscendCmp();
  void setupDescendCmp();
};
}



template <class T>
BOOM::VectorSorter<T>::VectorSorter(BOOM::Vector<T> &v,
				    BOOM::Comparator<T> &c)
  : theVector(v), numElements(v.size()), comp(c)
{
}



template <class T>
BOOM::Vector<T> *BOOM::VectorSorter<T>::quickSort()
{
  BOOM::Vector<T> &sorted=*new BOOM::Vector<T>(numElements);
  for(int i=0 ; i<numElements ; ++i) sorted[i]=theVector[i];
  quickSort(sorted,0,numElements-1);
  return &sorted;
}



template <class T>
void BOOM::VectorSorter<T>::setupAscendCmp()
{
  ptrToMmbrLess=&BOOM::Comparator<T>::less;
  ptrToMmbrGreater=&BOOM::Comparator<T>::greater;
}



template <class T>
void BOOM::VectorSorter<T>::setupDescendCmp()
{
  ptrToMmbrLess=&BOOM::Comparator<T>::greater;
  ptrToMmbrGreater=&BOOM::Comparator<T>::less;
}



template <class T>
BOOM::Vector<T> *BOOM::VectorSorter<T>::sortAscending()
{
  setupAscendCmp();
  return quickSort();
}



template <class T>
BOOM::Vector<T> *BOOM::VectorSorter<T>::sortDescending()
{
  setupDescendCmp();
  return quickSort();
}



template <class T>
BOOM::Vector<int> *BOOM::VectorSorter<T>::sortAscendByIndex()
{
  setupAscendCmp();
  return sortByIndex();
}



template <class T>
BOOM::Vector<int> *BOOM::VectorSorter<T>::sortDescendByIndex()
{
  setupDescendCmp();
  return sortByIndex();
}



template <class T>
void BOOM::VectorSorter<T>::sortAscendByIndex(BOOM::Vector<int> &into)
{
  setupAscendCmp();
  sortByIndex(into);
}



template <class T>
void BOOM::VectorSorter<T>::sortDescendByIndex(BOOM::Vector<int> &into)
{
  setupDescendCmp();
  sortByIndex(into);
}



template <class T>
void BOOM::VectorSorter<T>::sortByIndex(BOOM::Vector<int> &into)
{
  initIndexArray(into);
  quickSortIndices(into,theVector,0,numElements-1);
}



template <class T>
BOOM::Vector<int> *BOOM::VectorSorter<T>::sortByIndex()
{
  BOOM::Vector<int> &indices=*new BOOM::Vector<int>;
  sortByIndex(indices);
  return &indices;
}



template <class T>
int BOOM::VectorSorter<T>::partition(BOOM::Vector<T> &sorted,
				   int begin,int end)
{
   int pivotIndex=RandomNumber(end-begin+1)+begin;
   T temp=sorted[pivotIndex];
   //assert (pivotIndex<=sorted.size());
   swap(sorted,pivotIndex,begin);
   int i=begin-1;
   int j=end+1;
   while(true) 
     {
       do --j; while((comp.*ptrToMmbrGreater)(sorted[j],temp));
       do ++i; while((comp.*ptrToMmbrLess)(sorted[i],temp));
       if(i>=j) break;
       else swap(sorted,i,j);
     }
   return j;
}



template <class T>
void BOOM::VectorSorter<T>::initIndexArray(BOOM::Vector<int> &indices)
{
  for(int i=0 ; i<numElements ; ++i) indices.push_back(i);
}



template <class T>
void BOOM::VectorSorter<T>::quickSort(BOOM::Vector<T> &sorted,
			int from,int to)
{
  if(from>=to) return;
  int pivot=partition(sorted,from,to);
  quickSort(sorted,from,pivot);
  quickSort(sorted,pivot+1,to);
}



template <class T>
void BOOM::VectorSorter<T>::sortInPlace()
{
  quickSort(theVector,0,numElements-1);
}



template <class T>
void BOOM::VectorSorter<T>::sortAscendInPlace()
{
  setupAscendCmp();
  sortInPlace();
}



template <class T>
void BOOM::VectorSorter<T>::sortDescendInPlace()
{
  setupDescendCmp();
  return sortInPlace();
}



template <class T>
void BOOM::VectorSorter<T>::swap(BOOM::Vector<T> &data,int index1,
				 int index2)
{
  T temp=data[index1];
  data[index1]=data[index2];
  data[index2]=temp;
}



template <class T>
void BOOM::VectorSorter<T>::swapIndices(BOOM::Vector<int> &indices,
				      int index1,int index2)
{
  int temp=indices[index1];
  indices[index1]=indices[index2];
  indices[index2]=temp;
}



template <class T>
void BOOM::VectorSorter<T>::applyIndexMap(BOOM::Vector<int> &indices,
					BOOM::Vector<T> &from,
					BOOM::Vector<T> &to)
{
  int n=from.size();
  for(int i=0 ; i<n ; ++i)
    to[i]=from[indices[i]];
}



template <class T>
void BOOM::VectorSorter<T>::applyIndexMap(BOOM::Vector<int> &indices,
					BOOM::Vector<T> &fromAndTo)
{
  int n=fromAndTo.size();
  BOOM::Vector<T> temp(n);
  applyIndexMap(indices,fromAndTo,temp);
  for(int i=0 ; i<n ; ++i)
    fromAndTo[i]=temp[i];
}



template <class T>
void BOOM::VectorSorter<T>::quickSortIndices(BOOM::Vector<int> &indices,
					   BOOM::Vector<T> &data,
					   int from,int to)
{
  if(from>=to) return;
  int pivot=partitionIndices(indices,data,from,to);
  quickSortIndices(indices,data,from,pivot);
  quickSortIndices(indices,data,pivot+1,to);
}



template <class T>
int BOOM::VectorSorter<T>::partitionIndices(BOOM::Vector<int> &indices,
					  BOOM::Vector<T> &data,
					  int begin,int end)
{
   int pivotIndex=RandomNumber(end-begin+1)+begin;
   T temp=data[indices[pivotIndex]];
   swapIndices(indices,pivotIndex,begin);
   int i=begin-1;
   int j=end+1;
   while(true) 
     {
       do --j;
       while((comp.*ptrToMmbrGreater)(data[indices[j]],temp));
       do ++i;
       while((comp.*ptrToMmbrLess)(data[indices[i]],temp));
       if(i>=j) break;
       else swapIndices(indices,i,j);
     }
   return j;
}



template<class T>
void BOOM::VectorSorter<T>::shuffle(BOOM::Vector<T> &theVector)
{
  const int n=theVector.size()-1;
  for(int i=0 ; i<n ; ++i) {
    int j=i+RandomNumber(theVector.size()-i);
    T temp=theVector[i];
    theVector[i]=theVector[j];
    theVector[j]=temp;
  }
}


#endif
