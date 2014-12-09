/****************************************************************
 Poset.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Poset_H
#define INCL_Poset_H
#include <iostream>
#include "Vector.H"
using namespace std;
namespace BOOM {

/****************************************************************
 This is a POSET : a Partially Ordered SET.  It is basically the
 same as a directed graph.  Algorithms on Posets include:
   TopologicalSort
 ****************************************************************/
template<class T>
class Poset {
public:

template<class X>
  class Vertex {
    X data;
    int index;
    Vector<Vertex<X>*> successors;
  public:
    Vertex() {}
    Vertex(X x,int i) : data(x), index(i) {}
    X getData() const {return data;}
    void setData(X x) {data=x;}
    int getIndex() const {return index;}
    void addSuccessor(Vertex<X> *s) {successors.push_back(s);}
    int numSuccessors() const {return successors.size();}
    Vertex<X> *getIthSuccessor(int i) {return successors[i];}
  };

  Poset();
  virtual ~Poset();
  Vertex<T> &addVertex(T);
  int numVertices();
  Vertex<T> &getIthVertex(int);
  void dontDeleteVertices();

private:
  Vector<Vertex<T>*> vertices;
  bool shouldDelete;
};




/****************************************************************
                           Poset methods
 ****************************************************************/
template<class T>
Poset<T>::Poset() 
  : shouldDelete(true)
{
  // ctor
}



template<class T>
Poset<T>::~Poset<T>()
{
  if(shouldDelete) {
    int n=vertices.size();
    for(int i=0 ; i<n ; ++i) delete vertices[i];
  }
}



template<class T>
Poset<T>::Vertex<T> &Poset<T>::addVertex(T t)
{
  int i=vertices.size();
  vertices.push_back(new Vertex<T>(t,i));
  return *vertices[i];
}



template<class T>
int Poset<T>::numVertices()
{
  return vertices.size();
}



template<class T>
Poset<T>::Vertex<T> &Poset<T>::getIthVertex(int i)
{
  return *vertices[i];
}



template<class T>
void Poset<T>::dontDeleteVertices()
{
  shouldDelete=false;
}


};
#endif

