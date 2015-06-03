/***********************************************************************
 Vector.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Vector_H
#define INCL_BOOM_Vector_H
#include <iostream>
#include <vector>
#include "List.H"
#include "Random.H"
#include "Exceptions.H"
using namespace std;

namespace BOOM {
template<class T>
class Vector : public vector<T> {
public:
  Vector();
  Vector(int size);
  BOOM::Vector<T> *getSubrange(int from,int to);
  void getSubrange(int from,int to,BOOM::Vector<T> &);
  /*virtual T &operator[](int i) {
    if(i>=size() || i<0) throw "VECTOR";
    return (*(vector<T>*)this)[i];
  }
  virtual const T &operator[](int i) const {
    if(i>=size() || i<0) throw "VECTOR";
    return (*(vector<T>*)this)[i];
  }*/
  bool isMember(const T &);
  void append(const BOOM::Vector<T> &);
  void append(const BOOM::List<T> &);
  void push_front(const T &);
  void removeFirst(const T &); // removes first occurrence of given example
  bool isEmpty() const {return this->empty();}
  T getFirst() {return *this->begin();}
  T getLast() {return (*this)[this->size()-1];}
  void resize(int n) {vector<T>::resize(n);}
  void cut(int index);
  void cut(int startIndex,int howMany);
  void insertByIndex(const T &,int beforeIndex);
  void insertByIndex(const Vector<T> &,int beforeIndex);
  void reverse();
  inline void swap(int i,int j);
  void printOn(ostream &) const;
  void readFrom(istream &);
  void purge();
  T pop();
  void shuffle();
  BOOM::Vector<T> *uniq() const; // you should sort first...just like UNIX
  /*
    INHERITED METHODS:
    iterator begin();
    iterator end();
    void push_back(T);
    void pop_back(); // delete last element
    T &at(int i); // safer version of []
    T &front(); // first element
    T &back(); // last element
   */
};

typedef BOOM::Vector<int> IntVector;
typedef BOOM::Vector<float> FloatVector;
typedef BOOM::Vector<double> DoubleVector;
typedef BOOM::Vector<char> CharVector;
typedef BOOM::Vector<long> LongVector;
}


template<class T>
BOOM::Vector<T>::Vector()
{
}



template<class T>
BOOM::Vector<T>::Vector(int size) : vector<T>(size)
{
}



template<class T>
BOOM::Vector<T> *BOOM::Vector<T>::getSubrange(int from,int to)
{
  BOOM::Vector<T> &newVector=*new BOOM::Vector<T>;
  getSubrange(from,to,newVector);
  return &newVector;
}



template<class T>
bool BOOM::Vector<T>::isMember(const T &x)
{
  typename BOOM::Vector<T>::iterator cur=vector<T>::begin(), 
    end=vector<T>::end();
  for(; cur!=end ; ++cur)
    if(*cur==x) 
      return true;
  return false;
}



template<class T>
void BOOM::Vector<T>::append(const BOOM::Vector<T> &other)
{
  typename BOOM::Vector<T>::const_iterator cur=other.begin(), 
    end=other.end();
  for( ; cur!=end ; ++cur)
    push_back(*cur);
}



template<class T>
void BOOM::Vector<T>::append(const BOOM::List<T> &other)
{
  typename BOOM::List<T>::const_iterator cur=other.begin(), 
    end=other.end();
  for( ; cur!=end ; ++cur)
    push_back(*cur);
}



template<class T>
void BOOM::Vector<T>::getSubrange(int from,int to,BOOM::Vector<T> &into)
{
  BOOM::Vector<T> &self=*this;
  for(int i=from ; i<=to ; ++i)
    into.push_back(self[i]);
}



template<class T>
void BOOM::Vector<T>::push_front(const T &x)
{
  insert(vector<T>::begin(),x);
}



template<class T>
void BOOM::Vector<T>::removeFirst(const T &x)
{
  typename BOOM::Vector<T>::iterator cur=vector<T>::begin(), 
    end=vector<T>::end();
  for(; cur!=end ; ++cur)
    if(*cur==x) 
      {
	vector<T>::erase(cur);
	return;
      }
}



template<class T>
void BOOM::Vector<T>::cut(int index)
{
  cut(index,1);
}



template<class T>
void BOOM::Vector<T>::cut(int i,int howMany)
{
  BOOM::Vector<T> &self=*this;
  int n=vector<T>::size();
  int src=i+howMany;
  for(; src<n ; ++i, ++src)
    self[i]=self[src];
  resize(n-howMany);
}



template<class T>
void BOOM::Vector<T>::insertByIndex(const T &newT,int beforeIndex)
{
  int n=vector<T>::size();
  BOOM::Vector<T> &self=*this;
  push_back(newT);
  for(int i=n ; i>beforeIndex ; --i) self[i]=self[i-1];
  self[beforeIndex]=newT;
}



template<class T>
void BOOM::Vector<T>::swap(int i,int j)
{
  BOOM::Vector<T> &self=*this;
  T t=self[i];
  self[i]=self[j];
  self[j]=t;
}



template<class T>
void BOOM::Vector<T>::reverse()
{
  for(int i=0, j=vector<T>::size()-1 ; i<j ; ++i, --j)
    swap(i,j);
}



template<class T>
void BOOM::Vector<T>::purge()
{
  vector<T>::clear();
}



template<class T>
void BOOM::Vector<T>::printOn(ostream &os) const
{
  int n=vector<T>::size();
  os<<n<<endl;
  const Vector<T> &self=*this;
  for(int i=0 ; i<n ; ++i)
    os<<self[i]<<'\t';
}



template<class T>
void BOOM::Vector<T>::readFrom(istream &is)
{
  int n;
  is>>n;
  resize(n);
  Vector<T> &self=*this;
  for(int i=0 ; i<n ; ++i)
    is>>self[i];
}



template<class T>
ostream &operator<<(ostream &os,const BOOM::Vector<T> &v)
{
  v.printOn(os);
  return os;
}



template<class T>
istream &operator>>(istream &is,BOOM::Vector<T> &v)
{
  v.readFrom(is);
  return is;
}




template<class T>
void BOOM::Vector<T>::insertByIndex(const BOOM::Vector<T> &other,int pos)
{
  Vector<T> &self=*this;
  int L=vector<T>::size();
  int N=other.size();
  resize(L+N);
  int suffix=L-pos;
  for(int i=0 ; i<suffix ; ++i)
    self[L+N-1-i]=self[L-1-i];
  for(int i=0 ; i<N ; ++i)
    self[pos+i]=other[i];
}



template<class T>
T BOOM::Vector<T>::pop()
{
  T t=back();
  pop_back();
  return t;
}



template<class T>
void BOOM::Vector<T>::shuffle()
{
  Vector<T> &self=*this;
  int L=size();
  for(int i=0 ; i<L-1 ; ++i) {
    int j=RandomNumber(i,L-1);
    T temp=self[i];
    self[i]=self[j];
    self[j]=temp;
  }
}



template<class T>
BOOM::Vector<T> *BOOM::Vector<T>::uniq() const
{
  const BOOM::Vector<T> &self=*this;
  int newSize=0;
  for(typename BOOM::Vector<T>::const_iterator cur=begin(), end=self.end() ; 
      cur!=end ; ) {
    typename BOOM::Vector<T>::const_iterator next=cur;
    ++next;
    while(next!=end && (*next)==(*cur)) ++next;
    ++newSize;
    cur=next;
  }
  Vector<T> *r=new BOOM::Vector<T>();
  for(typename BOOM::Vector<T>::const_iterator cur=begin(), end=self.end() ; 
      cur!=end ; ) {
    r->push_back(*cur);
    typename BOOM::Vector<T>::const_iterator next=cur;
    ++next;
    while(next!=end && (*next)==(*cur)) ++next;
    cur=next;
  }
  return r;
}




#endif
