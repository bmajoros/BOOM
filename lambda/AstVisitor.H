/*
 AstVisitor.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_AstVisitor_H
#define INCL_AstVisitor_H

#include "AST.H"

using namespace std;
using namespace BOOM;

// Implements the "Visitor" design pattern, as described
// in the book "Design Patterns" by Erich Gamma, et al.

namespace Lambda {
class AstVisitor    // AstVisitor is just an interface class
{
public:
  virtual void visit(AstCombination &) {}
  virtual void visit(AstBoundVariable &) {}
  virtual void visit(AstFreeVariable &) {}
  virtual void visit(AstIntLit &) {}
  virtual void visit(AstFloatLit &) {}
  virtual void visit(AstLambda &) {}
  virtual void visit(AstStringLit &) {}
  virtual void visit(AstCharLit &) {}
  virtual void visit(AstSymbol &) {}
  virtual void visit(AstForest &) {}
  virtual void visit(AstObject &) {}
};

class AstPostorderTraversal : public AstVisitor {
public:
  // Call this function to initiate the traversal:
  void traverse(AstNode *);

  //  Override these in your derived class:
  virtual void process(AstCombination &) {}
  virtual void process(AstBoundVariable &) {}
  virtual void process(AstFreeVariable &) {}
  virtual void process(AstIntLit &) {}
  virtual void process(AstFloatLit &) {}
  virtual void process(AstLambda &) {}
  virtual void process(AstStringLit &) {}
  virtual void process(AstCharLit &) {}
  virtual void process(AstSymbol &) {}
  virtual void process(AstForest &) {}
  virtual void process(AstObject &) {}

  // Don't touch these at all:
  void visit(AstCombination &);
  void visit(AstBoundVariable &);
  void visit(AstFreeVariable &);
  void visit(AstIntLit &);
  void visit(AstFloatLit &);
  void visit(AstLambda &);
  void visit(AstStringLit &);
  void visit(AstCharLit &);
  void visit(AstSymbol &);
  void visit(AstForest &);
  void visit(AstObject &);
};

}

#endif
