/***********************************************************************
 List.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_List_H
#define INCL_BOOM_List_H
#include <list>
#include <functional>
using namespace std;

// A doubly-linked list class

namespace BOOM {
template<class T,class Compare=less<T> >
class List : public list<T> {
public:
  List() {}
  typename list<T>::iterator insertBefore(typename list<T>::iterator i,
					  const T &x)
  {return insert(i,x);}
  bool isEmpty() {return List<T>::empty();}
  int numElements() {return List<T>::size();}
  void append(const T &t) {insert(this->end(),t);}
  void append(const List<T> &other) {insert(this->end(),other.begin(),other.end());}
  void append(const typename list<T>::iterator i,const typename list<T>::iterator j) { list<T>::insert(this->end(),i,j); }
  void insertAfter(const T &t,typename list<T>::iterator after) {list<T>::insert(++after,t);}
  void prepend(const T &t) {push_front(t);}
  T &firstElem() {return this->front();}
  T &lastElem() {return back();}
  void removeFirstElem() {pop_front();}
  void removeLastElem() {pop_back();}
  void purge() {clear();}
  //void reverseOrder() {reverse();}
  //void removeDuplicates() {unique();}
  void sortInPlace(Compare comp=Compare()) {this->sort(comp);}//sort(begin(),end(),comp);}

  /* Inherited from list<T>:
     iterator begin();
     iterator end();
     void erase(iterator);
   */

  // *******
  // WARNING!  The following functions assume the list is already sorted:
  // *******
  BOOM::List<T,Compare> *intersectWith(BOOM::List<T,Compare> &);
  void intersectWith(BOOM::List<T,Compare> &,
		     BOOM::List<T,Compare> &destination);
  BOOM::List<T,Compare> *unionWith(BOOM::List<T,Compare> &);
  void unionWith(BOOM::List<T,Compare> &,
		 BOOM::List<T,Compare> &destination);
};



template<class T, class C>
void BOOM::List<T,C>::intersectWith(BOOM::List<T,C> &other,
				 BOOM::List<T,C> &destination)
{
  set_intersection(begin(),this->end(),other.begin(),other.end(),
		   inserter(destination,destination.begin()));
}



template<class T, class C>
BOOM::List<T,C> *BOOM::List<T,C>::intersectWith(BOOM::List<T,C> &other)
{
  BOOM::List<T,C> *newList=new BOOM::List<T,C>;
  intersectWith(other,*newList);
  return newList;
}



template<class T, class C>
void BOOM::List<T,C>::unionWith(BOOM::List<T,C> &other,
			       BOOM::List<T,C> &destination)
{
  set_union(begin(),this->end(),other.begin(),other.end(),
	    inserter(destination,destination.begin()));
}



template<class T, class C>
BOOM::List<T,C> *BOOM::List<T,C>::unionWith(BOOM::List<T,C> &other)
{
  BOOM::List<T,C> *newList=new BOOM::List<T,C>;
  unionWith(other,*newList);
  return newList;
}


}

#endif
