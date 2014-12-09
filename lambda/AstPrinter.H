/*
 AstPrinter.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_AstPrinter_H
#define INCL_AstPrinter_H

#include "AstVisitor.H"
using namespace std;
using namespace BOOM;

namespace Lambda {

class RunTimeEnvironment;

class AstPrinter : public AstVisitor
{
  int currentLexDepth;
  ostream &os;
  RunTimeEnvironment &env;
  int tabSpaces, tabSize;

  void incrementTab();
  void decrementTab();
  void tabOver();
public:
  AstPrinter(ostream &,RunTimeEnvironment &env);
  virtual void print(AstNode *);
  virtual void visit(AstCombination &);
  virtual void visit(AstBoundVariable &);
  virtual void visit(AstFreeVariable &);
  virtual void visit(AstIntLit &);
  virtual void visit(AstFloatLit &);
  virtual void visit(AstLambda &);
  virtual void visit(AstStringLit &);
  virtual void visit(AstCharLit &);
  virtual void visit(AstSymbol &);
  virtual void visit(AstForest &);
  virtual void visit(AstObject &);
};

}

#endif
