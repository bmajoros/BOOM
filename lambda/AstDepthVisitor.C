/*
 AstDepthVisitor.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "AstDepthVisitor.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

void AstDepthVisitor::visit(AstCombination &c,int depth) 
{
  visitChildren(c,depth);
}



void AstDepthVisitor::visit(AstLambda &l,int depth) 
{
  visitChildren(l,depth+1);
}



void AstDepthVisitor::visit(AstForest &f,int depth) 
{
  visitChildren(f,depth);
}



void AstDepthVisitor::visitChildren(AstCombination &combo,int depth)
{
  AstNode *function=combo.getFunction();
  function->acceptDepthVisitor(*this,depth);

  Array1DOf(AstNode*) &parms=combo.getParms();
  int n=parms.getSize();
  int i;
  for(i=0 ; i<n ; ++i)
    {
      AstNode *child=(AstNode*) parms[i];
      child->acceptDepthVisitor(*this,depth);
    }
}



void AstDepthVisitor::visitChildren(AstLambda &lambda,int depth)
{
  AstForest *forest=lambda.getBody();
  forest->acceptDepthVisitor(*this,depth+1);
}



void AstDepthVisitor::visitChildren(AstForest &forest,int depth)
{
  int n=forest.getNumTrees();
  int i;
  for(i=0 ; i<n ; ++i)
    {
      AstNode *child=forest.getIthTree(i);
      child->acceptDepthVisitor(*this,depth);
    }
}



void AstDepthVisitor::traverse(AstNode *node)
{
  node->acceptDepthVisitor(*this,0);
}



