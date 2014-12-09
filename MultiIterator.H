/****************************************************************
 MultiIterator.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_MultiIterator_H
#define INCL_MultiIterator_H
#include <iostream>
using namespace std;

namespace BOOM {

/****************************************************************
 MultiIterator : allows you to iterate through several collections
 in a seamless manner, using just one master iterator, without 
 having to explicitly merge those collections into a single 
 collection.
 ****************************************************************/
template<class T>
class MultiIterator {
public:
  MultiIterator();
  void append(InputIterator<T> begin,InputIterator<T> end);
  void start();
  bool done();
  T &operator*();
  void operator++();
protected:
  List<InputIterator<T> > begins, ends;
  List<InputIterator<T> >::iterator whichBegin, whichEnd;
  InputIterator<T> cur, end;
  void ensureValid();
};


template<class T>
MultiIterator<T>::MultiIterator()
  : whichBegin(begins.end())
{
  // ctor
}



template<class T>
void MultiIterator<T>::append(InputIterator<T> a,InputIterator<T> b)
{
  begins.push_back(a);
  ends.push_back(b);
}



template<class T>
void MultiIterator<T>::start()
{
  whichBegin=begins.begin();
  whichEnd=ends.begin();
  cur=*whichBegin;
  end=*whichEnd;
  ensureValid();
}



template<class T>
void MultiIterator<T>::ensureValid()
{
  while(cur==end && whichBegin!=begins.end()) {
    ++whichBegin;
    ++whichEnd;
    cur=*whichBegin;
    end=*whichEnd;
  }
}



template<class T>
void MultiIterator<T>::operator++()
{
  ++cur;
  ensureValid();
}



template<class T>
T &MultiIterator<T>::operator*()
{
  return *cur;
}



template<class T>
bool MultiIterator<T>::done()
{
  return whichBegin==begins.end();
}


}
#endif

