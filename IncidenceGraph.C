/****************************************************************
 IncidenceGraph.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "IncidenceGraph.H"
using namespace std;
using namespace BOOM;


IncidenceGraph::IncidenceGraph(int numVertices,int numEdges)
  : M(numEdges,numVertices)
{
  // ctor

  M.setAllTo(false);
}



Neighborhood *IncidenceGraph::getNeighborsOf(VertexId v,bool &shouldDelete)
  const
{
  shouldDelete=true;
  int numEdges=M.getFirstDim(), numVertices=M.getSecondDim();
  Neighborhood *H=new Neighborhood;
  for(int i=0 ; i<numEdges ; ++i)
    if(M[i][v])
      for(int j=0 ; j<numVertices ; ++j)
	if(j!=v && M[i][j])
	  {H->insert(j); break;}
  return H;
}



VertexId IncidenceGraph::addVertex()
{
  throw "Can't add vertices to an incidenceGraph; sorry";
}



bool IncidenceGraph::areAdjacent(VertexId a,VertexId b)
  const
{
  int numEdges=M.getFirstDim(), numVertices=M.getSecondDim();
  for(int i=0 ; i<numEdges ; ++i)
    if(M[i][a] && M[i][b])
      return true;
  return false;
}



int IncidenceGraph::getDegree(VertexId v) const
{
  int deg=0;
  int numEdges=M.getFirstDim();
  for(int i=0 ; i<numEdges ; ++i)
    if(M[i][v])
      ++deg;
  return deg;
}



int IncidenceGraph::getNumEdges() const
{
  return M.getFirstDim();
}



int IncidenceGraph::getNumVertices() const
{
  return M.getSecondDim();
}



void IncidenceGraph::addEdge(VertexId from,VertexId to)
{
  throw "Can't add edges to an IndicenceGraph; sorry";
}



void IncidenceGraph::removeEdge(VertexId from,VertexId to)
{
  throw "Can't remove edges from an IndicenceGraph; sorry";
}



