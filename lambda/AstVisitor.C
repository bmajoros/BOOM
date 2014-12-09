/*
 AstVisitor.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "AstVisitor.H"
using namespace Lambda;


void AstPostorderTraversal::traverse(AstNode *node)
{
  node->acceptVisitor(*this);
}



void AstPostorderTraversal::visit(AstCombination &node)
{
  AstNode *function=node.getFunction();
  Array1D<AstNode*> &parms=node.getParms();
  const int numParms=parms.size();
  for(int i=0 ; i<numParms ; ++i) parms[i]->acceptVisitor(*this);
  function->acceptVisitor(*this);
  process(node);
}



void AstPostorderTraversal::visit(AstBoundVariable &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstFreeVariable &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstIntLit &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstFloatLit &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstLambda &node)
{
  node.getBody()->acceptVisitor(*this);
  process(node);
}



void AstPostorderTraversal::visit(AstStringLit &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstCharLit &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstSymbol &node)
{
  process(node);
}



void AstPostorderTraversal::visit(AstForest &node)
{
  const int numTrees=node.getNumTrees();
  for(int i=0 ; i<numTrees ; ++i) 
    node.getIthTree(i)->acceptVisitor(*this);
  process(node);
}



void AstPostorderTraversal::visit(AstObject &node)
{
  process(node);
}

