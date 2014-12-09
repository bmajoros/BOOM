/*
 AstSerializer.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "AstSerializer.H"
#include <iostream.h>
#include "RunTimeEnvironment.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

// **************************************************
//               AstSerializer methods
// **************************************************

AstSerializer::AstSerializer(ostream &os,RunTimeEnvironment &env) 
  : os(os), currentLexDepth(0), env(env), tabSpaces(0),
    tabSize(3)
{
  // ctor
}



void AstSerializer::tabOver()
{
}



void AstSerializer::incrementTab()
{
}



void AstSerializer::decrementTab()
{
}



void AstSerializer::print(AstNode *node)
{
  currentLexDepth=0;
  tabSpaces=0;
  node->acceptVisitor(*this);
}



void AstSerializer::visit(AstCombination &combination)
{
  AstNode *function=combination.getFunction();
  Array1DOf(AstNode*) &parms=combination.getParms();

  tabOver();
  os << "(";
  incrementTab();
  function->acceptVisitor(*this);

  const int numParms=parms.getSize();
  int i;
  for(i=0 ; i<numParms ; ++i)
    {
      ((AstNode*)parms[i])->acceptVisitor(*this);
      os << ' ';
    }

  decrementTab();
  os << ")";
}



void AstSerializer::visit(AstBoundVariable &boundVar)
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
    //os << "(: " << depth << " " << position << ")";
    {
      char varName=char(int('a')+position);
      int absDepth=currentLexDepth-depth;
      os << varName << absDepth;
    }
}



void AstSerializer::visit(AstFreeVariable &freeVar)
{
  tabOver();
  os << freeVar.getLexeme();
}



void AstSerializer::visit(AstIntLit &intLit)
{
  tabOver();
  os << intLit.getValue();
}



void AstSerializer::visit(AstFloatLit &floatLit)
{
  tabOver();
  os << floatLit.getValue();
}



void AstSerializer::printFormalParms(ostream &os,int arity)
{
  int i;
  for(i=0 ; i<arity ; ++i)
    {
      char varName=char(int('a')+i);
      os << varName << currentLexDepth << ' ';
    }
}



void AstSerializer::visit(AstLambda &lambdaNode)
{
  tabOver();
  os << "[";

  ++currentLexDepth;
  printFormalParms(os,lambdaNode.getArity());

  os << "| ";

  lambdaNode.getBody()->acceptVisitor(*this);
  --currentLexDepth;

  tabOver();
  os << "]";
}



void AstSerializer::visit(AstStringLit &stringLit)
{
  tabOver();
  os << "\"" << stringLit.getValue() << "\"";
}



void AstSerializer::visit(AstCharLit &charLit)
{
  tabOver();
  os << "#" << charLit.getValue();
}



void AstSerializer::visit(AstSymbol &symbolNode)
{
  tabOver();
  os << "'" << symbolNode.getSymbol()->getLexeme();
}



void AstSerializer::visit(AstForest &astForest)
{
  const int numTrees=astForest.getNumTrees();
  int i;
  for(i=0 ; i<numTrees ; ++i)
    {
      AstNode *tree=astForest.getIthTree(i);
      tree->acceptVisitor(*this);
    }
}





