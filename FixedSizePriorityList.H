/***********************************************************************
 FixedSizePriorityList.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_FixedSizePriorityList_H
#define INCL_FixedSizePriorityList_H
using namespace std;
#include <iostream>
#include "List.H"
#include "Comparator.H"
#include "Iterator.H"
#include "Exceptions.H"

namespace BOOM {
template <class T>
class FSPLIterator : public BOOM::Iterator<T>
{
  typename BOOM::List<T>::iterator iter;
public:
  FSPLIterator(typename BOOM::List<T>::iterator);
  FSPLIterator() {}
  virtual void operator++();
  virtual void operator++(int);
  virtual T &operator*();
  virtual bool operator==(const BOOM::Iterator<T> &) const;
  virtual bool operator!=(const BOOM::Iterator<T> &i) const
    {return !(i==*this);}
  virtual BOOM::Iterator<T> &clone();
  operator typename BOOM::List<T>::iterator() {return iter;}
};



template <class T>
class FixedSizePriorityList
{
  BOOM::List<T> Q;
  BOOM::Comparator<T> *comp;
  int capacity;
  void force_insert(T);
public:
  FixedSizePriorityList(int capacity,
			BOOM::Comparator<T>* = new DirectComparator<T>());
  int getCapacity() const;
  int size() const;
  bool empty() const;
  bool insert(T); // <---can cause memory leak if you're not careful! ###
  bool push_back(T t) {return insert(t);}
  T &getMin();
  T &getMax();
  typedef FSPLIterator<T> iterator;
  iterator begin();
  iterator end();
  void erase(iterator);
  void clear() {Q.clear();}
  BOOM::List<T> &getList() {return Q;}
  void changeComparator(BOOM::Comparator<T> *c) {comp=c;}
  void sanityCheck();
};
}
using namespace BOOM;


template <class T>
FixedSizePriorityList<T>::FixedSizePriorityList(int capacity,
						BOOM::Comparator<T> *comp)
  : capacity(capacity),
    comp(comp)
{
}



template <class T>
int FixedSizePriorityList<T>::getCapacity() const
{
  return capacity;
}



template <class T>
int FixedSizePriorityList<T>::size() const
{
  return Q.size();
}



template <class T>
bool FixedSizePriorityList<T>::empty() const
{
  return Q.size()==0;
}



template <class T>
bool FixedSizePriorityList<T>::insert(T t)
{
  // If the list is not yet full, just insert the new element:
  if(Q.size()<capacity)
    {
      force_insert(t);
      return true;
    }

  // The list is at capacity.  If the new element is smaller than
  // even the minimum element, don't insert it:
  if(comp->less(t,getMin())) return false;

  // The new element deserves insertion.  Insert it and then delete
  // the smallest element from the list.
  force_insert(t);
  Q.erase(Q.begin());
  return true;
}



template <class T>
void FixedSizePriorityList<T>::force_insert(T t)
{
  bool inserted=false;
  typename BOOM::List<T>::iterator cur=Q.begin(), end=Q.end();
  for(; cur!=end ; ++cur)
    if(comp->less(t,*cur))
      {
	Q.insert(cur,t);
	inserted=true;
	break;
      }
  if(!inserted) Q.push_back(t);
  //sanityCheck();
}



template <class T>
void FixedSizePriorityList<T>::sanityCheck() 
{
  typename BOOM::List<T>::iterator cur=Q.begin(), end=Q.end();
  for(cur=Q.begin() ; cur!=end ; ++cur) cout<<**cur<<" ";
  cout<<endl;
  cur=Q.begin();
  for(; cur!=end ; ++cur) {
    typename BOOM::List<T>::iterator nxt=cur;
    ++nxt;
    if(nxt==end) break;
    if(comp->greater(*cur,*nxt)) {
      cout<<**cur<<" vs "<<**nxt<<endl;
      for(cur=Q.begin() ; cur!=end ; ++cur) cout<<**cur<<" ";
      cout<<endl;
      INTERNAL_ERROR;
      //throw "BAD!!!!";
    }
  }
}



template <class T>
T &FixedSizePriorityList<T>::getMin()
{
  return *Q.begin();
}



template <class T>
T &FixedSizePriorityList<T>::getMax()
{
  return *Q.rbegin();
}



template <class T>
typename 
  FixedSizePriorityList<T>::iterator FixedSizePriorityList<T>::begin()
{
  return FSPLIterator<T>(Q.begin());
}



template <class T>
typename FixedSizePriorityList<T>::iterator FixedSizePriorityList<T>::end()
{
  return FSPLIterator<T>(Q.end());
}



template <class T>
void FixedSizePriorityList<T>::erase(iterator i)
{
  Q.erase(i);
}



//================================================================
//                   FSPLIterator methods
//================================================================

template <class T>
FSPLIterator<T>::FSPLIterator(typename BOOM::List<T>::iterator iter)
  : iter(iter)
{
}



template <class T>
void FSPLIterator<T>::operator++()
{
  ++iter;
}



template <class T>
void FSPLIterator<T>::operator++(int)
{
  iter++;
}



template <class T>
T &FSPLIterator<T>::operator*()
{
  return *iter;
}



template <class T>
bool FSPLIterator<T>::operator==(const BOOM::Iterator<T> &other) const
{
  return iter==dynamic_cast<const FSPLIterator<T>&>(other).iter;
}



template <class T>
BOOM::Iterator<T> &FSPLIterator<T>::clone()
{
  return *new FSPLIterator(iter);
}


#endif
