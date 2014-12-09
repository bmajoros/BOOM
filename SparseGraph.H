/***********************************************************************
 SparseGraph.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_SparseGraph_H
#define INCL_BOOM_SparseGraph_H
#include "Vector.H"
#include "Graph.H"
#include "String.H"
#include "Map.H"
using namespace std;

namespace BOOM {

/***********************************************************************
 SparseGraph : efficiently represents a sparse graph using neighbor sets.
 Saves and loads TGF iles (Trivial Graph Format: list of vertices
 followed by list of edges).
 ***********************************************************************/

class SparseGraph : public Graph {
public:
  SparseGraph(int numVertices=0);
  SparseGraph(const String &filename);
  SparseGraph(istream &);
  void load(const String &filename);
  void load(istream &);
  void save(const String &filename) const;
  void save(ostream &) const;
  const String &getLabel(VertexId) const;
  virtual Neighborhood *getNeighborsOf(VertexId,bool &shouldDelete) const;
  virtual VertexId addVertex();
  virtual bool areAdjacent(VertexId,VertexId) const;
  virtual int getDegree(VertexId) const;
  virtual int getNumEdges() const;
  virtual int getNumVertices() const;
  virtual void addEdge(VertexId from,VertexId to);
  virtual void removeEdge(VertexId from,VertexId to);
  void getEdges(Vector< pair<VertexId,VertexId> > &);
private:
  int numEdges;
  BOOM::Vector<Neighborhood> neighborhoods;
  BOOM::Vector<String> vertexLabels;
  BOOM::Map<String,VertexId> labelMap;
};
}

#endif
