/***********************************************************************
 SparseGraph.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include <fstream>
#include "SparseGraph.H"
using namespace std;
using namespace BOOM;


BOOM::SparseGraph::SparseGraph(int numVertices)
  : numEdges(0)
{
  for(int i=0 ; i<numVertices ; ++i) 
    addVertex();
}



SparseGraph::SparseGraph(const String &filename)
{
  load(filename);
}



SparseGraph::SparseGraph(istream &is)
{
  load(is);
}



void SparseGraph::load(const String &filename)
{
  ifstream is(filename.c_str());
  load(is);
}



void SparseGraph::load(istream &is)
{
  bool readingEdges=false;
  String line;
  while(!is.eof()) {
    line.getline(is);
    line.trimWhitespace();
    if(line.isEmpty()) continue;
    if(line[0]=='#') {readingEdges=true;continue;}
    BOOM::Vector<BOOM::String> &fields=*line.getFields();
    if(readingEdges) {
      if(fields.size()<2) throw String("incorrect edge format: ")+line;
      const String &v1=fields[0], &v2=fields[1];
      if(!labelMap.isDefined(v1)) throw v1+" is not defined";
      if(!labelMap.isDefined(v2)) throw v2+" is not defined";
      VertexId id1=labelMap[v1], id2=labelMap[v2];
      Neighborhood &n=neighborhoods[id1];
      n.insert(id2);
    }
    else {
      const String &label=fields[0];
      VertexId id=addVertex();
      labelMap[label]=id;
      vertexLabels[id]=label;
    }
    delete &fields;
  }
}



const String &SparseGraph::getLabel(VertexId id) const
{
  return vertexLabels[id];
}



void SparseGraph::save(const String &filename) const
{
  ofstream os(filename.c_str());
  save(os);
}



void SparseGraph::save(ostream &os) const
{
  int n=getNumVertices();
  for(VertexId v=0 ; v<n ; ++v) os<<v<<" "<<vertexLabels[v]<<endl;
  os<<"#"<<endl;
  for(VertexId v=0 ; v<n ; ++v) {
    const Neighborhood &neighbors=neighborhoods[v];
    Neighborhood::const_iterator cur=neighbors.begin(), end=neighbors.end();
    for(; cur!=end ; ++cur) {
      VertexId neighbor=*cur;
      os<<v<<"\t"<<neighbor<<endl;
    }
  }
}



Neighborhood *BOOM::SparseGraph::getNeighborsOf(VertexId vertexId,
						bool &shouldDelete) const
{
  shouldDelete=false;
  return &const_cast<SparseGraph*>(this)->neighborhoods[vertexId];
}



VertexId BOOM::SparseGraph::addVertex()
{
  VertexId id=neighborhoods.size();
  neighborhoods.push_back(Neighborhood());
  vertexLabels.push_back("");
  return id;
}



bool BOOM::SparseGraph::areAdjacent(VertexId v1,VertexId v2) const
{
  const Neighborhood &v1neighbors=neighborhoods[v1];
  bool notFound=v1neighbors.find(v2)==v1neighbors.end();
  return !notFound;
}



int BOOM::SparseGraph::getDegree(VertexId vertex) const
{
  int degree=neighborhoods[vertex].size();
  return degree;
}



int BOOM::SparseGraph::getNumEdges() const
{
  return numEdges;
}



int BOOM::SparseGraph::getNumVertices() const
{
  return neighborhoods.size();
}



void BOOM::SparseGraph::addEdge(VertexId v1,VertexId v2)
{
  Neighborhood &v1neighbors=neighborhoods[v1];
  if(v1neighbors.find(v2)==v1neighbors.end())
    {
      v1neighbors.insert(v2);
      Neighborhood &v2neighbors=neighborhoods[v2];
      v2neighbors.insert(v1);
      ++numEdges;
    }
}



void BOOM::SparseGraph::removeEdge(VertexId v1,VertexId v2)
{
  Neighborhood &v1neighbors=neighborhoods[v1];
  Neighborhood::iterator cur=v1neighbors.find(v2);
  bool found=(cur!=v1neighbors.end());
  if(found)
    {
      v1neighbors.erase(cur);
      Neighborhood &v2neighbors=neighborhoods[v2];
      v2neighbors.erase(v2neighbors.find(v1));
      --numEdges;
    }
}



void SparseGraph::getEdges(Vector< pair<VertexId,VertexId> > &edges)
{
  int n=getNumVertices();
  for(VertexId v=0 ; v<n ; ++v) {
    const Neighborhood &neighbors=neighborhoods[v];
    Neighborhood::const_iterator cur=neighbors.begin(), end=neighbors.end();
    for(; cur!=end ; ++cur) {
      VertexId neighbor=*cur;
      edges.push_back(pair<VertexId,VertexId>(v,neighbor));
    }
  }
}


