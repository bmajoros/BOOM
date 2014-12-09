/***********************************************************************
 WindowedQueue.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_WindowedQueue_H
#define INCL_BOOM_WindowedQueue_H
#include <list>
using namespace std;

namespace BOOM {
template<class T>
class WindowedQueue
{
public:
  WindowedQueue();
  bool isEmpty();
  list<T> &getReservoir();
  list<T> &getWindow();
  void enqueue(T);
  void shift();
private:
  list<T> reservoir, window;
};
}



template<class T>
BOOM::WindowedQueue<T>::WindowedQueue()
{
   
}



template<class T>
bool BOOM::WindowedQueue<T>::isEmpty()
{
  return window.empty() && reservoir.empty();
}



template<class T>
list<T> &BOOM::WindowedQueue<T>::getReservoir()
{
  return reservoir;
}



template<class T>
list<T> &BOOM::WindowedQueue<T>::getWindow()
{
  return window;
}



template<class T>
void BOOM::WindowedQueue<T>::enqueue(T t)
{
  window.push_back(t);
}



template<class T>
void BOOM::WindowedQueue<T>::shift()
{
  reservoir.push_back(window.front());
  window.pop_front();
}

#endif
