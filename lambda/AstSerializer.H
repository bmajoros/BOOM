/*
 AstSerializer.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_AstSerializer_H
#define INCL_AstSerializer_H

#include "AstVisitor.H"
using namespace std;
using namespace BOOM;

namespace Lambda {

class RunTimeEnvironment;

class AstSerializer : public AstVisitor
{
  int currentLexDepth;
  ostream &os;
  RunTimeEnvironment &env;
  int tabSpaces, tabSize;

  void incrementTab();
  void decrementTab();
  void tabOver();
  void AstSerializer::printFormalParms(ostream &os,int arity);
public:
  AstSerializer(ostream &,RunTimeEnvironment &env);
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
};

}

#endif
