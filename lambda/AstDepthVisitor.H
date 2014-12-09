/*
 AstDepthVisitor.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_AstDepthVisitor_H
#define INCL_AstDepthVisitor_H

#include "AST.H"
using namespace std;
using namespace BOOM;

namespace Lambda {

/*
 Implements the "Visitor" design pattern, as described
 in the book "Design Patterns" by Erich Gamma, et al.
 Differs from an AstVisitor in that lexical depths
 are tracked.
*/

class AstDepthVisitor
{
protected:
  virtual void visitChildren(AstCombination &,int depth);
  virtual void visitChildren(AstLambda &,int depth);
  virtual void visitChildren(AstForest &,int depth);
public:
  // Call this to traverse the tree
  virtual void traverse(AstNode *);

  // Callback functions from the AstNode subclasses
  virtual void visit(AstCombination &c,int depth);
  virtual void visit(AstLambda &l,int depth);
  virtual void visit(AstForest &f,int depth);
  virtual void visit(AstBoundVariable &,int depth) {}
  virtual void visit(AstFreeVariable &,int depth) {}
  virtual void visit(AstIntLit &,int depth) {}
  virtual void visit(AstFloatLit &,int depth) {}
  virtual void visit(AstStringLit &,int depth) {}
  virtual void visit(AstCharLit &,int depth) {}
  virtual void visit(AstSymbol &,int depth) {}
  virtual void visit(AstObject &,int depth) {}
};

}

#endif
