/****************************************************************
 MinimalSpanningTree.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_MinimalSpanningTree_H
#define INCL_MinimalSpanningTree_H
#include <iostream>
#include "EdgeWeights.H"
#include "SparseGraph.H"
#include "Comparator.H"
using namespace std;

namespace BOOM {

class MinimalSpanningTree
{
public:
  MinimalSpanningTree(const Graph &,const EdgeWeights &);
  Graph *getTree();
  const EdgeWeights &getEdgeWeights();
private:
  struct Edge {
    int vertex1, vertex2;
    double weight;
    Edge(int a,int b,double w) : vertex1(a), vertex2(b), weight(w) {}
  };
  struct EdgeComparator : public Comparator<Edge*>
  {
    bool equal(Edge *&a,Edge *&b)   
      {return a->vertex1==b->vertex1 && a->vertex2==b->vertex2 &&
	  a->weight==b->weight;}
    bool greater(Edge *&a,Edge *&b) 
    {
      return 
	a->weight>b->weight || 
	  a->weight==b->weight && 
	      (a->vertex1>b->vertex1 || 
                 a->vertex1==b->vertex1 && a->vertex2>b->vertex2);
    }
    bool less(Edge *&a,Edge *&b)    
    {
      return 
	a->weight<b->weight || 
	  a->weight==b->weight && 
	      (a->vertex1<b->vertex1 || 
                 a->vertex1==b->vertex1 && a->vertex2<b->vertex2);
    }
  };
  SparseGraph *G;
  const Graph &fullGraph;
  const EdgeWeights &E;
  int numVertices;

  void construct();
};

}

#endif

