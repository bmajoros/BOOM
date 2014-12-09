/****************************************************************
 IncidenceGraph.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_IncidenceGraph_H
#define INCL_IncidenceGraph_H
#include <iostream>
#include "Graph.H"
#include "Array2D.H"
using namespace std;


namespace BOOM {

class IncidenceGraph : public Graph
{
public:
  IncidenceGraph(int numVertices,int numEdges);
  virtual Neighborhood *getNeighborsOf(VertexId,bool &shouldDelete) const;
  virtual VertexId addVertex();
  virtual bool areAdjacent(VertexId,VertexId) const;
  virtual int getDegree(VertexId) const;
  virtual int getNumEdges() const;
  virtual int getNumVertices() const;
  virtual void addEdge(VertexId from,VertexId to);
  virtual void removeEdge(VertexId from,VertexId to);
private:
  Array2D<bool> M; // incidence matrix : M[edge][vertex]
};

}

#endif

