/***********************************************************************
 ListQueue.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_ListQueue_H
#define INCL_BOOM_ListQueue_H
#include <list>
using namespace std;

namespace BOOM {
template<class T>
class ListQueue
{
public:
  ListQueue();
  T &back();
  T &front();
  typename list<T>::iterator begin();
  typename list<T>::iterator end();
  typename list<T>::reverse_iterator rbegin(); 
  typename list<T>::reverse_iterator rend(); 
  void enqueue(const T &);
  T dequeue() {T &t=back();pop_back();return t;}
  void pop_back();
  void pop_front();
  int size();
  void clear();
  bool isEmpty() {return theList.empty();}
private:
  list<T> theList;
};
}



template<class T>
int BOOM::ListQueue<T>::size()
{
  return theList.size();
}



template<class T>
void BOOM::ListQueue<T>::clear()
{
  theList.clear();
}



template<class T>
T &BOOM::ListQueue<T>::back()
{
  return theList.back();
}



template<class T>
T &BOOM::ListQueue<T>::front()
{
  return theList.front();
}



template<class T>
BOOM::ListQueue<T>::ListQueue() 
{
   
}



template<class T>
typename list<T>::iterator BOOM::ListQueue<T>::begin()
{
  return theList.begin();
}



template<class T>
typename list<T>::iterator BOOM::ListQueue<T>::end()
{
  return theList.end();
}



template<class T>
typename list<T>::reverse_iterator BOOM::ListQueue<T>::rbegin()
{
  return theList.rbegin();
}



template<class T>
typename list<T>::reverse_iterator BOOM::ListQueue<T>::rend()
{
  return theList.rend();
}



template<class T>
void BOOM::ListQueue<T>::enqueue(const T &t)
{
  theList.push_back(t);
}



template<class T>
void BOOM::ListQueue<T>::pop_back()
{
  theList.pop_back();
}



template<class T>
void BOOM::ListQueue<T>::pop_front()
{
  theList.pop_front();
}

#endif
