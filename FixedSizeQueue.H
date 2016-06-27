/****************************************************************
 FixedSizeQueue.H
 Copyright (C)2016 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_FixedSizeQueue_H
#define INCL_FixedSizeQueue_H
#include <iostream>
#include "List.H"
using namespace std;
using namespace BOOM;

namespace BOOM {
template<class T>
class FixedSizeQueue {
public:
  FixedSizeQueue(int maxSize=0);
  void resize(int maxSize) { this->maxSize=maxSize; }
  void enqueue(T);
  typename List<T>::iterator begin();
  typename List<T>::iterator end();
  bool isFull() const;
  bool isEmpty() const;
  int numElements() const { return q.size(); }
  void purge() { q.purge(); }
private:
  List<T> q;
  int maxSize;
};
}



template<class T>
FixedSizeQueue<T>::FixedSizeQueue(int maxSize)
  : maxSize(maxSize)
{
}



template<class T>
void FixedSizeQueue<T>::enqueue(T t)
{
  q.append(t);
  if(q.size()>maxSize) q.removeFirstElem();
}



template<class T>
typename List<T>::iterator FixedSizeQueue<T>::begin()
{
  return q.begin();
}



template<class T>
typename List<T>::iterator FixedSizeQueue<T>::end()
{
  return q.end();
}



template<class T>
bool FixedSizeQueue<T>::isFull() const
{
  return q.size()>=maxSize;
}



template<class T>
bool FixedSizeQueue<T>::isEmpty() const
{
  return q.isEmpty();
}




#endif

