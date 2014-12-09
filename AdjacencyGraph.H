/****************************************************************
 AdjacencyGraph.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_AdjacencyGraph_H
#define INCL_AdjacencyGraph_H
#include <iostream>
#include "Graph.H"
#include "Array2D.H"
using namespace std;

namespace BOOM {

class AdjacencyGraph : public Graph
{
public:
  AdjacencyGraph(int numVertices);
  virtual Neighborhood *getNeighborsOf(VertexId,bool &shouldDelete) const;
  virtual VertexId addVertex();
  virtual bool areAdjacent(VertexId,VertexId) const;
  virtual int getDegree(VertexId) const;
  virtual int getNumEdges() const;
  virtual int getNumVertices() const;
  virtual void addEdge(VertexId from,VertexId to);
  virtual void removeEdge(VertexId from,VertexId to);
private:
  Array2D<bool> M; // adjacency matrix
  int numVertices, numEdges;
};

}

#endif

