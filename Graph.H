/****************************************************************
 Graph.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_Graph_H
#define INCL_Graph_H
#include <iostream>
#include "Set.H"
using namespace std;
using namespace BOOM;

namespace BOOM {

typedef unsigned VertexId;
typedef BOOM::Set<VertexId> Neighborhood;

class Graph
{
public:
  virtual Neighborhood *getNeighborsOf(VertexId,bool &shouldDelete) const=0;
  virtual VertexId addVertex()=0;
  virtual bool areAdjacent(VertexId,VertexId) const=0;
  virtual int getDegree(VertexId) const=0;
  virtual int getNumEdges() const=0;
  virtual int getNumVertices() const=0;
  virtual void addEdge(VertexId from,VertexId to)=0;
  virtual void removeEdge(VertexId from,VertexId to)=0;
};

}

#endif

