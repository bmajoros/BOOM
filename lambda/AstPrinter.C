/*
 AstPrinter.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "AstPrinter.H"
#include <iostream>
#include "RunTimeEnvironment.H"
#include "Closure.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

AstPrinter::AstPrinter(ostream &os,RunTimeEnvironment &env) 
  : os(os), currentLexDepth(0), env(env), tabSpaces(0),
    tabSize(3)
{
  // ctor
}



void AstPrinter::tabOver()
{
  int i;
  for(i=0 ; i<tabSpaces ; ++i)
    os << " ";
}



void AstPrinter::incrementTab()
{
  tabSpaces+=tabSize;
}



void AstPrinter::decrementTab()
{
  tabSpaces-=tabSize;
}



void AstPrinter::print(AstNode *node)
{
  currentLexDepth=0;
  tabSpaces=0;
  node->acceptVisitor(*this);
}



void AstPrinter::visit(AstCombination &combination)
{
  AstNode *function=combination.getFunction();
  Array1D<AstNode*> &parms=combination.getParms();

  tabOver();
  os << "(" << endl;
  incrementTab();
  function->acceptVisitor(*this);
  os << endl;

  const int numParms=parms.size();
  int i;
  for(i=0 ; i<numParms ; ++i)
    {
      ((AstNode*)parms[i])->acceptVisitor(*this);
      if(i<numParms-1) 
	os << endl;
    }

  decrementTab();
  //tabOver();
  os << ")";
}



void AstPrinter::visit(AstBoundVariable &boundVar)
{
  tabOver();

  int depth=boundVar.getLexAddr().getDepth();
  int position=boundVar.getLexAddr().getPosition();

  if(depth==currentLexDepth)
    {
      // It is a global, so look it up in the global table and
      // print out its name instead of the lexical address

      os << env.getGlobalName(position);
    }
  else
    os << "(: " << depth << " " << position << ")";
}



void AstPrinter::visit(AstFreeVariable &freeVar)
{
  tabOver();
  os << "free:" << freeVar.getLexeme();
}



void AstPrinter::visit(AstIntLit &intLit)
{
  tabOver();
  os << intLit.getValue();
}



void AstPrinter::visit(AstFloatLit &floatLit)
{
  tabOver();
  os << floatLit.getValue();
}



void AstPrinter::visit(AstLambda &lambdaNode)
{
  tabOver();
  os << "[" << lambdaNode.getArity()
    << "|" << endl;

  incrementTab();
  ++currentLexDepth;
  lambdaNode.getBody()->acceptVisitor(*this);
  --currentLexDepth;
  decrementTab();

  tabOver();
  os << "]";
}



void AstPrinter::visit(AstStringLit &stringLit)
{
  tabOver();
  os << "\"" << stringLit.getValue() << "\"";
}



void AstPrinter::visit(AstCharLit &charLit)
{
  tabOver();
  os << "#" << charLit.getValue();
}



void AstPrinter::visit(AstSymbol &symbolNode)
{
  tabOver();
  os << "'" << symbolNode.getSymbol()->getLexeme();
}



void AstPrinter::visit(AstForest &astForest)
{
  const int numTrees=astForest.getNumTrees();
  int i;
  for(i=0 ; i<numTrees ; ++i)
    {
      AstNode *tree=astForest.getIthTree(i);
      tree->acceptVisitor(*this);
      os << endl;
    }
}



void AstPrinter::visit(AstObject &node)
{
  LambdaObject *obj=node.getObject();
  tabOver();
  if(obj->getType()==OBTYPE_CLOSURE) {
    Closure *closure=static_cast<Closure*>(obj);
    closure->getFunction()->acceptVisitor(*this);
  }
  else os<<obj->getType();
}



