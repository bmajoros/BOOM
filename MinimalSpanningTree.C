/****************************************************************
 MinimalSpanningTree.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "MinimalSpanningTree.H"
#include "Set.H"
#include "Array1D.H"
#include "BitSet.H"
#include "PriorityTree.H"
#include "Graph.H"
using namespace std;
using namespace BOOM;


MinimalSpanningTree::MinimalSpanningTree(const Graph &g,
					 const EdgeWeights &e)
  : E(e),
    fullGraph(g),
    numVertices(g.getNumVertices()),
    G(new SparseGraph(g.getNumVertices()))
{
  construct();
}



Graph *MinimalSpanningTree::getTree()
{
  return G;
}



const EdgeWeights &MinimalSpanningTree::getEdgeWeights()
{
  return E;
}



void MinimalSpanningTree::construct()
{
  // Add all the edges to a priority queue
  EdgeComparator cmp;
  PriorityTree<Edge*> Q(cmp);
  for(VertexId i=0 ; i<numVertices ; ++i)
    {
      bool shouldDelete;
      Neighborhood *n=fullGraph.getNeighborsOf(i,shouldDelete);
      BOOM::Set<VertexId>::iterator cur=n->begin(), end=n->end();
      for(; cur!=end ; ++cur)
	{
	  VertexId v=*cur;
	  double weight=E.getWeight(i,v);
	  Q.insert(new Edge(i,v,weight));
	}
      if(shouldDelete) delete n;
    }

  // Iteratively select the best edge whose vertices are not both 
  // in the same tree
  Array1D<BitSet*> trees(numVertices);
  trees.setAllTo(NULL);
  while(!Q.isEmpty())
    {
      Edge *e=Q.extractMin();
      int v1=e->vertex1, v2=e->vertex2;
      BitSet *&s1=trees[v1], *&s2=trees[v2];
      if(s1==NULL)
	if(s2==NULL)
	  {
	    s1=s2=new BitSet(numVertices);
	    s1->addMember(v1);
	    s1->addMember(v2);
	  }
	else
	  {
	    s2->addMember(v1);
	    s1=s2;
	  }
      else 
	if(s2==NULL)
	  {
	    s1->addMember(v2);
	    s2=s1;
	  }
	else
	  {
	    if(s1==s2) {delete e; continue;} // would create a cycle
	    
	    // Merge s1 and s2 by allowing s1 to gobble up s2
	    (*s1)+=(*s2);
	    for(int i=0 ; i<numVertices ; ++i)
	      if(s2->isMember(i))
		trees[i]=s1;
	    delete s2;
	  }
      delete e;
      G->addEdge(v1,v2);
    }
}



