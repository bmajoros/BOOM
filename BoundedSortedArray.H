/***********************************************************************
 BoundedSortedArray.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BoundedSortedArray_H
#define INCL_BoundedSortedArray_H

#include "Array1D.H"

namespace BOOM {
enum KeepType
  {
    KEEP_LOW_VALUES,
    KEEP_HIGH_VALUES
  };

template<class ObjectType,class ScalarType>
class BoundedSortedArray
{
  struct Element
  {
    ObjectType object;
    ScalarType scalar;
  };
  BOOM::Array1D<Element> array;
  KeepType keepType;
  int maxSize, currentSize;
  int search(ScalarType);
public:
  BoundedSortedArray(int size,KeepType);
  bool add(const ObjectType &,ScalarType);
  ObjectType &operator[](int);
  const ObjectType &operator[](int) const;
  ScalarType getValue(int i) const {return array[i].scalar;}
  int size();
};



template<class ObjectType,class ScalarType>
BoundedSortedArray<ObjectType,ScalarType>::BoundedSortedArray(int size,
							      KeepType k)
  : array(size), keepType(k), maxSize(size), currentSize(0)
{
  // ctor
}



template<class ObjectType,class ScalarType>
bool BoundedSortedArray<ObjectType,ScalarType>::add(const ObjectType &o,
						    ScalarType s)
{
  // Determine where the new object belongs -- returned index is always >=0,
  // and is either an empty slot, or a slot past the right end of the array,
  // or the location of an object which is > this one.
  int index=search(s);

  // If the array is under capacity, just insert it there
  if(currentSize<maxSize)
    {
      if(index<currentSize)
	for(int i=currentSize ; i>index ; --i)
	  array[i]=array[i-1];

      Element &e=array[index];
      e.object=o;
      e.scalar=s;
      ++currentSize;
    }

  // Otherwise, lose an object on the left or right end of the array, shift
  // everything over, and then insert the new object
  else
    {
      if(keepType==KEEP_LOW_VALUES)
	for(int i=maxSize-1 ; i>index ; --i)
	  array[i]=array[i-1];
      else 
      // keepType==KEEP_HIGH_VALUES
	{
	  --index;
	  if(index<0) return false;
	  for(int i=0 ; i<index ; ++i)
	    array[i]=array[i+1];
	}

      if(index<maxSize)
	{
	  Element &e=array[index];
	  e.object=o;
	  e.scalar=s;
	}
      else return false;
    }
  return true;
}



template<class ObjectType,class ScalarType>
ObjectType &BoundedSortedArray<ObjectType,ScalarType>::operator[](int i)
{
  return array[i].object;
}



template<class ObjectType,class ScalarType>
const ObjectType &BoundedSortedArray<ObjectType,ScalarType>::operator[](
							      int i) const
{
  return array[i].object;
}



template<class ObjectType,class ScalarType>
int BoundedSortedArray<ObjectType,ScalarType>::search(ScalarType s)
{
  /*
    Returns the index of a slot which is either undefined, or is
    strictly greater than s.
   */

  if(currentSize==0) return 0;
  int min=0, max=currentSize;
  while(min<max)
    {
      int mid=(min+max)/2;
      ScalarType midValue=array[mid].scalar;
      if(s<midValue) max=mid;
      else min=mid+1;
    }
  return min;
}



template<class ObjectType,class ScalarType>
int BoundedSortedArray<ObjectType,ScalarType>::size()
{
  return currentSize;
}
}


#endif
