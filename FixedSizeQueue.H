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
  FixedSizeQueue(int maxSize);
  void enqueue(T);
  List<T>::iterator begin();
  List<T>::iterator end();
  bool isFull() const;
  bool isEmpty() const;
private:
  List<T> q;
  int maxSize;
};
}


template<class T>
FixedSizeQueue::FixedSizeQueue(int maxSize)
{
}



template<class T>
void FixedSizeQueue::enqueue(T)
{
}



template<class T>
List<T>::iterator FixedSizeQueue::begin()
{
}



template<class T>
List<T>::iterator FixedSizeQueue::end()
{
}



template<class T>
bool FixedSizeQueue::isFull() const
{
}



template<class T>
bool FixedSizeQueue::isEmpty() const
{
}




#endif

