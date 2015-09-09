/***********************************************************************
 Set.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Set_H
#define INCL_BOOM_Set_H
#include <iostream>
#include <set>
#include <algorithm>
#include <ostream>
using namespace std;

namespace BOOM {
template< class T, class C = less<T> >
class Set : public set<T,C> {
public:
  Set() {}
  bool operator+=(const BOOM::Set<T,C> &);
  bool operator+=(const T &);
  bool isMember(const T &);
  void remove(const T &t);
  BOOM::Set<T,C> *intersectWith(BOOM::Set<T,C> &);
  void intersectWith(BOOM::Set<T,C> &,BOOM::Set<T,C> &destination);
  BOOM::Set<T,C> *unionWith(BOOM::Set<T,C> &);
  void unionWith(BOOM::Set<T,C> &,BOOM::Set<T,C> &destination);
  inline bool isEmpty() const {return empty();}
  void printOn(ostream &) const;
  T getSingleElement() const; // throws exception if size()!=1
};

template<class T, class C>
ostream &operator<<(ostream &,const Set<T,C> &);

}
using namespace BOOM;



template<class T, class C>
T BOOM::Set<T,C>::getSingleElement() const
{
  if(size()!=1) throw "Set size not 1 in Set<T,C>::getSingleElement()";
  return *begin();
}



template<class T, class C>
void BOOM::Set<T,C>::intersectWith(BOOM::Set<T,C> &other,
				 BOOM::Set<T,C> &destination)
{
  set_intersection(begin(),end(),other.begin(),other.end(),
		   inserter(destination,destination.begin()));
}



template<class T, class C>
BOOM::Set<T,C> *BOOM::Set<T,C>::intersectWith(BOOM::Set<T,C> &other)
{
  BOOM::Set<T,C> *newSet=new BOOM::Set<T,C>;
  intersectWith(other,*newSet);
  return newSet;
}



template<class T, class C>
void BOOM::Set<T,C>::unionWith(BOOM::Set<T,C> &other,
			       BOOM::Set<T,C> &destination)
{
  set_union(begin(),end(),other.begin(),other.end(),
	    inserter(destination,destination.begin()));
}



template<class T, class C>
BOOM::Set<T,C> *BOOM::Set<T,C>::unionWith(BOOM::Set<T,C> &other)
{
  BOOM::Set<T,C> *newSet=new BOOM::Set<T,C>;
  unionWith(other,*newSet);
  return newSet;
}



template<class T, class C>
bool BOOM::Set<T,C>::isMember(const T &t)
{
  return this->find(t)!=this->end();
}



template<class T, class C>
bool BOOM::Set<T,C>::operator+=(const T &other)
{
  if(!isMember(other))
    {
      insert(other);
      return true;
    }
  return false;
}



template<class T, class C>
bool BOOM::Set<T,C>::operator+=(const BOOM::Set<T,C> &other)
{
  bool changes=false;
  typename BOOM::Set<T,C>::iterator cur=other.begin(), end=other.end();
  for(; cur!=end ; ++cur)
    if(!isMember(*cur))
      {
	insert(*cur);
	changes=true;
      }
  return false;
}



template<class T, class C>
void BOOM::Set<T,C>::remove(const T &t) 
{
  erase(t);
}



template<class T, class C>
void BOOM::Set<T,C>::printOn(ostream &os) const
{
  os<<"[";
  typename Set<T,C>::const_iterator cur=begin(), e=end();
  for(; cur!=e ; ++cur)
    os<<*cur<<" ";
  os<<"]";
}



template<class T, class C>
ostream &BOOM::operator<<(ostream &os,const Set<T,C> &s)
{
  s.printOn(os);
  return os;
}

#endif
