/****************************************************************
 TopologicalSort.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_TopologicalSort_H
#define INCL_TopologicalSort_H
#include <iostream>
#include "Poset.H"
#include "Array1D.H"
#include "Stack.H"
using namespace std;
namespace BOOM {

#define DEBUG 0

/****************************************************************
                     class TopologicalSort
  ****************************************************************/
template<class T>
class TopologicalSort {
public:
  typedef typename Poset<T>::template Vertex<T> Node;
  Array1D<typename Poset<T>::template Vertex<T>*> *sort(const Poset<T> &);
private:
  typedef pair<Node*,int> StackElem;
  void dfs(Node &,Vector<Node*> &,BitSet &);
};




/****************************************************************
                    TopologicalSort methods
 ****************************************************************/
template<class T>
Array1D<typename Poset<T>::template Vertex<T>*> *
   TopologicalSort<T>::sort(const Poset<T> &P)
{
  int numVertices=P.numVertices();
  Vector<Node*> V;
  BitSet seen(numVertices);
  for(int i=0 ; i<numVertices ; ++i) {
    if(!seen.isMember(i)) {
      dfs(P.getIthVertex(i),V,seen);
    }	
  }
  V.reverse();
  return new Array1D<Node*>(V);
}



template<class T>
void TopologicalSort<T>::dfs(Node &root,Vector<Node*> &V,BitSet &seen)
{
  Stack<StackElem> S;
  S.push(StackElem(&root,0));
  seen.addMember(root.getIndex());
                                      if(DEBUG) cout<<"d2 "<<&root<<endl;
  while(!S.isEmpty()) {
                                             if(DEBUG) cout<<"d3"<<endl;
    int topIndex=S.size()-1;
                                     if(DEBUG) cout<<"d4 "<<topIndex<<endl;
    StackElem &top=S.peekTop();
                                                 if(DEBUG) cout<<"d5"<<endl;
    Node *topNode=top.first;
                                      if(DEBUG) cout<<"d6 "<<topNode<<endl;
    int nextChild=top.second;
                                     if(DEBUG) cout<<"d7 "<<nextChild<<endl;
    if(nextChild<topNode->numSuccessors()) {
                       if(DEBUG) cout<<"d8 "<<topNode->numSuccessors()<<endl;
      Node *child=topNode->getIthSuccessor(nextChild);
             if(DEBUG) cout<<"d9 "<<child<<" index="<<child->getIndex()<<endl;
      if(!seen.isMember(child->getIndex())) {
	                                         if(DEBUG) cout<<"d10"<<endl;
	S.push(StackElem(child,0));
	                                         if(DEBUG) cout<<"d11"<<endl;
        seen.addMember(child->getIndex());
      }	
                                                 if(DEBUG) cout<<"d12"<<endl;
      ++S.asVector()[topIndex].second;
                                                 if(DEBUG) cout<<"d13"<<endl;
    }
    else {
                                                  if(DEBUG) cout<<"d14"<<endl;
      S.pop();
                                                  if(DEBUG) cout<<"d15"<<endl;
      V.push_back(topNode);
                                                  if(DEBUG) cout<<"d16"<<endl;
    }
  }
}



};
#endif

