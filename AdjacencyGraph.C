/****************************************************************
 AdjacencyGraph.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "AdjacencyGraph.H"
using namespace std;
using namespace BOOM;

AdjacencyGraph::AdjacencyGraph(int numVertices)
  : M(numVertices,numVertices),
    numVertices(numVertices),
    numEdges(0)
{
  // ctor

  M.setAllTo(false);
}



Neighborhood *AdjacencyGraph::getNeighborsOf(VertexId v,bool &shouldDelete)
  const
{
  shouldDelete=false;
  Neighborhood *H=new Neighborhood;
  for(int i=0 ; i<numVertices ; ++i)
    if(M[v][i])
      H->insert(i);
  return H;
}



VertexId AdjacencyGraph::addVertex()
{
  throw String("Cannot add vertices to an AdjacencyGraph; sorry.");
}



bool AdjacencyGraph::areAdjacent(VertexId a,VertexId b) const
{
  return M[a][b];
}



int AdjacencyGraph::getDegree(VertexId v) const
{
  int deg=0;
  for(int i=0 ; i<numVertices ; ++i)
    if(M[v][i])
      ++deg;
  return deg;
}



int AdjacencyGraph::getNumEdges() const
{
  return numEdges;
}



int AdjacencyGraph::getNumVertices() const
{
  return numVertices;
}



void AdjacencyGraph::addEdge(VertexId from,VertexId to)
{
  bool &e=M[from][to];
  if(!e) ++numEdges;
  e=true;
}



void AdjacencyGraph::removeEdge(VertexId from,VertexId to)
{
  bool &e=M[from][to];
  if(e) --numEdges;
  e=false;
}


