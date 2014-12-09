/*
 AST.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
/*
  Contents:
  
  enum AST_NodeType
  class AstNode
  class AstForest
  class AstCombination
  class AstPrimary
  class AstLiteral
  class AstVariable
  class AstBoundVariable
  class AstFreeVariable
  class AstIntLit
  class AstFloatLit
  class AstLambda
  class AstStringLit
  class AstCharLit
  class AstSymbol
 */

#ifndef INCL_AST_H
#define INCL_AST_H

#include "BOOM/ElasVector.H"
#include "BOOM/Array1D.H"
#include "BOOM/String.H"
#include "LexicalAddress.H"
#include "Symbol.H"
#include "Garbage.H"

using namespace std;
using namespace BOOM;

namespace Lambda {

class AstFreeVariable;
class AstBoundVariable;
class AstVisitor;
class AstDepthVisitor;

// **************************************************
//                 enum AST_NodeType
// **************************************************

/*
 The type of a node in an Abstract Syntax Tree.
 This is stored explicitly because using virtual
 methods to handle polymorphism would get
 in the way of tail-recursion-optimization, which
 is a central concern in the architecture of the
 interpreter.
*/

enum AST_NodeType
{
  AST_COMBINATION,
  AST_BOUND_VARIABLE,
  AST_FREE_VARIABLE,
  AST_INT_LIT,
  AST_FLOAT_LIT,
  AST_LAMBDA,
  AST_STRING_LIT,
  AST_CHAR_LIT,
  AST_SYMBOL,
  AST_FOREST,
  AST_OBJECT // ### 
};

ostream &operator<<(ostream &,AST_NodeType);


// **************************************************
//                 class AstNode
// **************************************************

// Abstract

class AstNode : public Garbage
{
  AST_NodeType nodeType;
public:
  AstNode(AST_NodeType);
  virtual ~AstNode();
  AST_NodeType getNodeType() const;
  virtual void bindVar(AstFreeVariable *,AstBoundVariable *) {}
  virtual void acceptVisitor(AstVisitor &)=0;
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth)=0;
  virtual void setParent(AstNode *) {}
  virtual AstNode *deepCopy()=0;
  virtual int depth()=0;
};

typedef AstNode LambdaExpression;



// **************************************************
//                 class AstForest
// **************************************************

class AstForest : public AstNode
{
  ElasVector<AstNode*> trees;
  int numTrees;
public:
  AstForest();
  void appendTree(AstNode *);
  void changeTree(int index,AstNode *newTree);
  void deleteTree(int index);//lets go of it...doesn't delete it
  void insertTree(int index,AstNode *newTree);
  int getNumTrees();
  AstNode *getIthTree(int);
  virtual void bindVar(AstFreeVariable *,AstBoundVariable *);
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &);
  virtual AstNode *deepCopy();
  virtual int depth();
};



// **************************************************
//             class AstCombination
// **************************************************

// A function call.

class AstCombination : public AstNode
{
  AstNode *function;
  Array1D<AstNode*> &parms;
public:
  AstCombination(AstNode *function,Array1D<AstNode*> *parms);
  ~AstCombination();
  virtual void bindVar(AstFreeVariable *,AstBoundVariable *);
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  AstNode *getFunction();
  void changeFunction(AstNode *);
  Array1D<AstNode*> &getParms();
  virtual void pushAccessibles(MarkStack &);
  virtual AstNode *deepCopy();
  virtual int depth();
};


// **************************************************
//                class AstPrimary
// **************************************************

// Abstract.  Must be AstLiteral or AstVariable.

class AstPrimary : public AstNode
{
protected:
  AstPrimary(AST_NodeType);
};


// **************************************************
//                class AstLiteral
// **************************************************

// Abstract.  Must be AstIntLit, AstFloatLit, AstStringLit,
// AstLambda, or AstSymbol.

class AstLiteral : public AstPrimary
{
protected:
  AstLiteral(AST_NodeType);
};



// **************************************************
//                class AstObject
// **************************************************

// A lambda object -- usually used for embedding a 
// closure in an AST, when that closure was actually
// defined elsewhere.

class AstObject : public AstLiteral 
{
  LambdaObject *obj;
public:
  AstObject(LambdaObject *obj) : AstLiteral(AST_OBJECT), obj(obj) {}
  void changeObject(LambdaObject *newObj) { obj=newObj; }
  LambdaObject *getObject() const { return obj; }
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &);
  virtual AstNode *deepCopy() {return this;}
  virtual int depth() {}
};



// **************************************************
//                class AstVariable
// **************************************************

// Abstract.  Must be AstFreeVariable or AstBoundVariable.

class AstVariable : public AstPrimary
{
protected:
  AstVariable(AST_NodeType);
};



// **************************************************
//             class AstBoundVariable
// **************************************************

// Normal variable, represented by a lexical address.

class AstBoundVariable : public AstVariable
{
  LexicalAddress lexAddr;
public:
  AstBoundVariable(LexicalAddress);
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  LexicalAddress &getLexAddr();
  virtual void pushAccessibles(MarkStack &) {}
  virtual AstNode *deepCopy();
  virtual int depth();
  virtual void changeAddress(const LexicalAddress &);
};


// **************************************************
//               class AstFreeVariable
// **************************************************

// A free variable.  It is an error to encounter this
// during execution.  During backpatching, gets replaced
// by an AstBoundVariable.

class AstFreeVariable : public AstVariable
{
  String lexeme;
  int lexicalDepth;
  AstNode *parent;// for replacing me with an AstBoundVariable
public:
  AstFreeVariable(const String &lexeme,int lexicalDepth,
		  AstNode *parent);
  AstNode *getParent() const;
  virtual void setParent(AstNode *);
  int getLexicalDepth() const;
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &);
  const String &getLexeme() const;
  virtual AstNode *deepCopy();
  virtual int depth();
};



// **************************************************
//                class AstIntLit
// **************************************************

class AstIntLit : public AstLiteral
{
  int value;
public:
  AstIntLit(int);
  int getValue() const;
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &) {}
  virtual AstNode *deepCopy();
  virtual int depth();
  virtual void changeValue(int newValue) { value=newValue; }
};


// **************************************************
//                class AstFloatLit
// **************************************************

class AstFloatLit : public AstLiteral
{
  float value;
public:
  AstFloatLit(float);
  float getValue() const;
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &) {}
  virtual AstNode *deepCopy();
  virtual int depth();
  virtual void changeValue(float newValue) { value=newValue; }
};


// **************************************************
//                class AstLambda
// **************************************************

class AstLambda : public AstLiteral
{
  int arity;
  AstForest *body;
public:
  AstLambda(int arity,AstForest *body);
  ~AstLambda();
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  int getArity() const;
  AstForest *getBody();
  void changeBody(AstForest *);
  virtual void pushAccessibles(MarkStack &);
  virtual AstNode *deepCopy();
  virtual int depth();
};

typedef AstLambda LambdaFunction;



// **************************************************
//                class AstStringLit
// **************************************************

class AstStringLit : public AstLiteral
{
  String value;
public:
  AstStringLit(const String &);
  const String &getValue() const;
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &) {}
  virtual AstNode *deepCopy();
  virtual int depth();
};


// **************************************************
//                class AstCharLit
// **************************************************

class AstCharLit : public AstLiteral
{
  char value;
public:
  AstCharLit(char);
  char getValue() const;
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  virtual void pushAccessibles(MarkStack &) {}
  virtual AstNode *deepCopy();
  virtual int depth();
};


// **************************************************
//                class AstSymbol
// **************************************************

class AstSymbol : public AstLiteral
{
  Symbol *symbol;
public:
  AstSymbol(Symbol *);
  virtual void acceptVisitor(AstVisitor &);
  virtual void acceptDepthVisitor(AstDepthVisitor &,int depth);
  Symbol *getSymbol();
  virtual void pushAccessibles(MarkStack &);
  virtual AstNode *deepCopy();
  virtual int depth();
};

}

#endif
