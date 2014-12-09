/***********************************************************************
 PriorityTree.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_PriorityTree_H
#define INCL_BOOM_PriorityTree_H
#include "FastBinTree.H"
#include "Comparator.H"
using namespace std;

namespace BOOM {

template <class T>
class PriorityTree {
public:
  PriorityTree(Comparator<T> &);
  T extractMin();
  T extractMax();
  const T &peekMin() const;
  const T &peekMax() const;
  void insert(const T &);
  void remove(const T &t) {tree.remove(t);} // ### untested
  int size() const;
  bool isEmpty() const;
  void purge() { tree.purge(); numElements=0; }
  void enableDuplicates() {tree.enableDuplicates();}//### untested
private:
  FastBinTree<T> tree;
  int numElements;
};
}
using namespace BOOM;



template <class T>
PriorityTree<T>::PriorityTree(Comparator<T> &cmp)
  : numElements(0),
    tree(cmp)
{
  // ctor
}



template <class T>
T BOOM::PriorityTree<T>::extractMin()
{
  T t=tree.minimum();
  tree.remove(t);
  --numElements;
  return t;
}



template <class T>
T BOOM::PriorityTree<T>::extractMax()
{
  T t=tree.maximum();
  tree.remove(t);
  --numElements;
  return t;
}



template <class T>
const T &BOOM::PriorityTree<T>::peekMin() const
{
  return tree.minimum();
}



template <class T>
const T &BOOM::PriorityTree<T>::peekMax() const
{
  return tree.maximum();
}



template <class T>
void BOOM::PriorityTree<T>::insert(const T &t)
{
  tree.insert(t);
  ++numElements;
}



template <class T>
int BOOM::PriorityTree<T>::size() const
{
  //cout<<"tree.cardinality="<<tree.getCardinality()<<endl;
  return numElements;
}



template <class T>
bool BOOM::PriorityTree<T>::isEmpty() const
{
  return numElements==0;
}


#endif
