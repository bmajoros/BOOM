/***********************************************************************
 Stack.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Stack_H
#define INCL_BOOM_Stack_H
#include "Vector.H"
using namespace std;

namespace BOOM {
template<class T>
class Stack
{
public:
  Stack();
  Stack(BOOM::Vector<T> &);
  virtual ~Stack();
  void push(const T &);
  T pop(); // removes & returns the top element!
  void discardTop(); // removes top element
  const T &top();
  const T &bottom();
  T &peekTop();
  int size() const;
  bool isEmpty() const;
  typedef typename BOOM::Vector<T>::iterator iterator;
  typedef typename BOOM::Vector<T>::reverse_iterator reverse_iterator;
  iterator begin();
  iterator end();
  reverse_iterator rbegin();
  reverse_iterator rend();
  BOOM::Vector<T> &asVector() {return theStack;}
  void clear() {theStack.clear();}
private:
  BOOM::Vector<T> &theStack;
  bool shouldDelete;
};
}


template<class T>
BOOM::Stack<T>::Stack()
  : theStack(*new BOOM::Vector<T>),
    shouldDelete(true)
{
}



template<class T>
BOOM::Stack<T>::Stack(BOOM::Vector<T> &v)
  : theStack(v),
    shouldDelete(false)
{
}



template<class T>
BOOM::Stack<T>::~Stack()
{
  if(shouldDelete) delete &theStack;
}



template<class T>
void BOOM::Stack<T>::push(const T &t)
{
  theStack.push_back(t);
}



template<class T>
T BOOM::Stack<T>::pop()
{
  T t=theStack.back();
  theStack.resize(theStack.size()-1);
  return t;
}



template<class T>
void BOOM::Stack<T>::discardTop()
{
  theStack.resize(theStack.size()-1);
}



template<class T>
const T &BOOM::Stack<T>::top()
{
  return theStack.back();
}



template<class T>
T &BOOM::Stack<T>::peekTop()
{
  return theStack.back();
}



template<class T>
const T &BOOM::Stack<T>::bottom()
{
  return theStack[0];
}



template<class T>
int BOOM::Stack<T>::size() const
{
  return theStack.size();
}



template<class T>
bool BOOM::Stack<T>::isEmpty() const
{
  return theStack.size()==0;
}



template<class T>
typename BOOM::Stack<T>::iterator BOOM::Stack<T>::begin()
{
  return theStack.begin();
}



template<class T>
typename BOOM::Stack<T>::iterator BOOM::Stack<T>::end()
{
  return theStack.end();
}



template<class T>
typename BOOM::Stack<T>::reverse_iterator BOOM::Stack<T>::rbegin()
{
  return theStack.rbegin();
}



template<class T>
typename BOOM::Stack<T>::reverse_iterator BOOM::Stack<T>::rend()
{
  return theStack.rend();
}


#endif
