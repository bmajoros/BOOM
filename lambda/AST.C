/*
 AST.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "AST.H"
#include "LambdaException.H"
#include "AstVisitor.H"
#include "AstDepthVisitor.H"
#include <iostream>
using namespace std;
using namespace BOOM;
using namespace Lambda;


ostream &Lambda::operator<<(ostream &os,AST_NodeType t)
{
  switch(t)
    {
    case AST_COMBINATION:    os<<"combination"; break;
    case AST_BOUND_VARIABLE: os<<"bound-variable"; break;
    case AST_FREE_VARIABLE:  os<<"free-variable"; break;
    case AST_INT_LIT:        os<<"int"; break;
    case AST_FLOAT_LIT:      os<<"float"; break;
    case AST_LAMBDA:         os<<"lambda"; break;
    case AST_STRING_LIT:     os<<"string"; break;
    case AST_CHAR_LIT:       os<<"char"; break;
    case AST_SYMBOL:         os<<"symbol"; break;
    case AST_FOREST:         os<<"forest"; break;
    case AST_OBJECT:         os<<"embedded-object"; break;
    default: throw "Unknown AST_NodeType";
    }
  return os;
}



// **************************************************
//                 AstForest methods
// **************************************************

int AstForest::depth()
{
  int maxChildDepth=0;

  int i;
  for(i=0 ; i<numTrees ; ++i)
    {
      const int childDepth=((AstNode*)trees[i])->depth();
      if(childDepth > maxChildDepth)
	maxChildDepth=childDepth;
    }

  return maxChildDepth+1;
}



AstNode *AstForest::deepCopy()
{
  AstForest *copy=new AstForest;

  int i;
  for(i=0 ; i<numTrees ; ++i)
    copy->appendTree(((AstNode*)trees[i])->deepCopy());

  return copy;
}



void AstForest::pushAccessibles(MarkStack &markStack)
{
  int i;
  for(i=0 ; i<numTrees ; ++i)
    markStack.push((Garbage*)trees[i]);
}



void AstForest::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



void AstForest::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstForest::bindVar(AstFreeVariable *freeVar,
			AstBoundVariable *boundVar)
{
  int i;
  for(i=0 ; i<numTrees ; ++i)
    if(trees[i]==freeVar)
      {
	trees[i]=boundVar;
	return;
      }
  throw LambdaException("Can't find free var in AstForest");
}



AstForest::AstForest()
  : trees(NULL), numTrees(0), AstNode(AST_FOREST)
{
  // ctor
}



void AstForest::appendTree(AstNode *tree)
{
  trees[numTrees]=tree;
  ++numTrees;
  tree->setParent(this);
}



int AstForest::getNumTrees()
{
  return numTrees;
}



AstNode *AstForest::getIthTree(int i)
{
  if(i<0 || i>= numTrees)
    throw LambdaException("Invalid index in AstForest::getIthTree");

  return (AstNode *) trees[i];
}



void AstForest::changeTree(int index,AstNode *newTree)
{
  trees[index]=newTree;
  newTree->setParent(this);
}



void AstForest::deleteTree(int index)
{
  int i;
  for(i=index ; i<numTrees ; ++i)
    trees[i]=trees[i+1];
  --numTrees;
}



void AstForest::insertTree(int index,AstNode *newTree)
{
  int i;
  for(i=index ; i<numTrees ; ++i)
    trees[i+1]=trees[i];
  trees[i]=newTree;
  ++numTrees;
  newTree->setParent(this);
}



// **************************************************
//                   AstNode methods
// **************************************************

AstNode::AstNode(AST_NodeType nodeType)
  : nodeType(nodeType)
{
  // ctor
}



AstNode::~AstNode()
{
  // dtor
}



AST_NodeType AstNode::getNodeType() const
{
  return nodeType;
}



// **************************************************
//             AstCombination methods
// **************************************************

int AstCombination::depth()
{
  int maxChildDepth=function->depth();

  int i;
  for(i=0 ; i<parms.size() ; ++i)
    {
      const int childDepth=((AstNode*)parms[i])->depth();
      if(childDepth > maxChildDepth)
	maxChildDepth=childDepth;
    }

  return maxChildDepth+1;
}



AstNode *AstCombination::deepCopy()
{
  Array1D<AstNode*> *copyParms=new Array1D<AstNode*>(parms.size());
  int i;
  for(i=0 ; i<parms.size() ; ++i)
    (*copyParms)[i]=((AstNode*)parms[i])->deepCopy();

  return new AstCombination(function->deepCopy(),copyParms);
}



void AstCombination::pushAccessibles(MarkStack &markStack)
{
  int i;
  for(i=0 ; i<parms.size() ; ++i)
    markStack.push((Garbage *) parms[i]);
  
  markStack.push(function);//shallower -> processed sooner
}



void AstCombination::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



void AstCombination::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



AstCombination::AstCombination(AstNode *function,
			       Array1D<AstNode*> *parms)
  : function(function), parms(*parms), AstNode(AST_COMBINATION)
{
  // ctor

  if(function)// <--necessary for genetic programming!
    function->setParent(this);
  int i;
  for(i=0 ; i<parms->size() ; ++i)
    if((*parms)[i])
      ((AstNode*)(*parms)[i])->setParent(this);
}



void AstCombination::changeFunction(AstNode *newFunction)
{
  delete function; // ###
  function=newFunction;
  if(function)// <--necessary for genetic programming!
    function->setParent(this);
}



AstCombination::~AstCombination()
{
  //delete function; // no! GC will delete it later
  delete &parms;
}



void AstCombination::bindVar(AstFreeVariable *freeVar,
			     AstBoundVariable *boundVar)
{
  if(function==freeVar)
    function=boundVar;
  else
    {
      const int num=parms.size();
      for(int i=0 ; i<num ; ++i)
	if(parms[i]==freeVar)
	  {
	    parms[i]=boundVar;
	    return;
	  }
      //throw LambdaException("Can't find free var in AstCombination");
    }
}



AstNode *AstCombination::getFunction()
{
  return function;
}



Array1D<AstNode*> &AstCombination::getParms()
{
  return parms;
}



// **************************************************
//                 AstPrimary methods
// **************************************************

AstPrimary::AstPrimary(AST_NodeType t)
  : AstNode(t)
{
  // protected ctor
}



// **************************************************
//                 AstLiteral methods
// **************************************************

AstLiteral::AstLiteral(AST_NodeType t)
  : AstPrimary(t)
{
  // protected ctor
}



// **************************************************
//                 AstVariable methods
// **************************************************

AstVariable::AstVariable(AST_NodeType t)
  : AstPrimary(t)
{
  // protected ctor
}



// **************************************************
//              AstBoundVariable methods
// **************************************************

int AstBoundVariable::depth()
{
  return 1;
}



void AstBoundVariable::changeAddress(const LexicalAddress &la)
{
  lexAddr=la;
}



AstNode *AstBoundVariable::deepCopy()
{
  return new AstBoundVariable(lexAddr);
}



void AstBoundVariable::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstBoundVariable::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstBoundVariable::AstBoundVariable(LexicalAddress lexAddr)
  : lexAddr(lexAddr), AstVariable(AST_BOUND_VARIABLE)
{
  // ctor
}



LexicalAddress &AstBoundVariable::getLexAddr()
{
  return lexAddr;
}



// **************************************************
//                AstFreeVariable methods
// **************************************************

int AstFreeVariable::depth()
{
  return 1;
}



AstNode *AstFreeVariable::deepCopy()
{
  return new AstFreeVariable(lexeme,lexicalDepth,parent);
}



const String &AstFreeVariable::getLexeme() const
{
  return lexeme;
}



void AstFreeVariable::pushAccessibles(MarkStack &markStack)
{
  markStack.push(parent);
}



void AstFreeVariable::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstFreeVariable::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstFreeVariable::AstFreeVariable(const String &lexeme,
				 int lexicalDepth,AstNode *parent)
  : lexicalDepth(lexicalDepth), parent(parent),
    AstVariable(AST_FREE_VARIABLE), lexeme(lexeme)
{
  // ctor
}



AstNode *AstFreeVariable::getParent() const
{
  return parent;
}



int AstFreeVariable::getLexicalDepth() const
{
  return lexicalDepth;
}



void AstFreeVariable::setParent(AstNode *node)
{
  parent=node;
}



// **************************************************
//                 AstIntLit methods
// **************************************************

int AstIntLit::depth()
{
  return 1;
}



AstNode *AstIntLit::deepCopy()
{
  return new AstIntLit(value);
}



void AstIntLit::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstIntLit::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstIntLit::AstIntLit(int value)
  : value(value), AstLiteral(AST_INT_LIT)
{
  // ctor
}



int AstIntLit::getValue() const
{
  return value;
}



// **************************************************
//                 AstFloatLit methods
// **************************************************

int AstFloatLit::depth()
{
  return 1;
}



AstNode *AstFloatLit::deepCopy()
{
  return new AstFloatLit(value);
}



void AstFloatLit::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstFloatLit::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstFloatLit::AstFloatLit(float value)
  : value(value), AstLiteral(AST_FLOAT_LIT)
{
  // ctor
}



float AstFloatLit::getValue() const
{
  return value;
}



// **************************************************
//                 AstLambda methods
// **************************************************

int AstLambda::depth()
{
  return body->depth()+1;
}



AstNode *AstLambda::deepCopy()
{
  return new AstLambda(arity,
    static_cast<AstForest*>(body->deepCopy()));
}



void AstLambda::pushAccessibles(MarkStack &markStack)
{
  markStack.push(body);
}



void AstLambda::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstLambda::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstLambda::AstLambda(int arity,AstForest *body)
  : arity(arity), body(body), AstLiteral(AST_LAMBDA)
{
  // ctor
}



AstLambda::~AstLambda()
{
  //delete body; // no! GC will delete it later
}



int AstLambda::getArity() const
{
  return arity;
}



AstForest *AstLambda::getBody()
{
  return body;
}



void AstLambda::changeBody(AstForest *newBody)
{
  body=newBody;
}



// **************************************************
//                 AstStringLit methods
// **************************************************

int AstStringLit::depth()
{
  return 1;
}



AstNode *AstStringLit::deepCopy()
{
  return new AstStringLit(value);
}



void AstStringLit::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstStringLit::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstStringLit::AstStringLit(const String &value)
  : value(value), AstLiteral(AST_STRING_LIT)
{
  // ctor
}



const String &AstStringLit::getValue() const
{
  return value;
}



// **************************************************
//                 AstCharLit methods
// **************************************************

int AstCharLit::depth()
{
  return 1;
}



AstNode *AstCharLit::deepCopy()
{
  return new AstCharLit(value);
}



void AstCharLit::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstCharLit::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstCharLit::AstCharLit(char value)
  : value(value), AstLiteral(AST_CHAR_LIT)
{
  // ctor
}



char AstCharLit::getValue() const
{
  return value;
}



// **************************************************
//                 AstSymbol methods
// **************************************************

int AstSymbol::depth()
{
  return 1;
}



AstNode *AstSymbol::deepCopy()
{
  return new AstSymbol(symbol);
}



void AstSymbol::acceptDepthVisitor(AstDepthVisitor &visitor,int depth)
{
  visitor.visit(*this,depth);
}



void AstSymbol::acceptVisitor(AstVisitor &visitor)
{
  visitor.visit(*this);
}



AstSymbol::AstSymbol(Symbol *symbol)
  : symbol(symbol), AstLiteral(AST_SYMBOL)
{
  // ctor
}



Symbol *AstSymbol::getSymbol()
{
  return symbol;
}



void AstSymbol::pushAccessibles(MarkStack &markStack)
{
  //### might have to uncomment this later, but 
  // for now, symbols are not handled by the GC at all
  markStack.push(symbol); // ###
}



// **************************************************
//                 AstObject methods
// **************************************************
void AstObject::acceptVisitor(AstVisitor &v)
{
  v.visit(*this);
}



void AstObject::acceptDepthVisitor(AstDepthVisitor &v,int depth)
{
  v.visit(*this,depth);
}



void AstObject::pushAccessibles(MarkStack &v)
{
  if(obj) v.push(obj);
}







