/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */

#ifndef INCL_LambdaAPI_H
#define INCL_LambdaAPI_H

#include "RunTimeEnvironment.H"
#include "Evaluator.H"
#include "AST.H"
#include "LambdaInt.H"
#include "LambdaFloat.H"
#include "LambdaChar.H"
#include "LambdaString.H"
#include "ForeignFunctionObject.H"
using namespace std;
using namespace BOOM;

/*************************************************************
   A programmatic interface to the interpreter.  Using
   this class, you can define new lambda functions and
   objects that are then available to the lambda runtime 
   evironment.  Your lambda functions should be defined
   like so:

   LambdaObject *f(RunTimeEnvironment &E,ContinuationType &) {
     LambdaObject *parm1=env.getParm(0);
     LambdaObject *parm2=env.getParm(1);
     ...etc...
     return result;
   }

   Then just call LambdaAPI::defineFunction(f,"name",arity);

   To make your C++ objects available to lambda programs
   (inside a lambda object wrapper), see class "ForeignObject" 
   in LambdaObject.H.
*************************************************************/

namespace Lambda {

// **************************************************
//                class LambdaAPI
// **************************************************

class LambdaAPI
{
  RunTimeEnvironment &env;
  Evaluator &evaluator;
  unsigned randomSeed;

public:
  LambdaAPI(unsigned randomSeed=0);
  virtual ~LambdaAPI();

  // For parsing source code
  AstForest *parseFile(const String &filename,bool regWithGC=false);
  AstForest *parseSource(const String &sourceCode,bool regWithGC=false);

  // For evaluating expressions (once they've been parsed)
  LambdaObject *evaluate(AstNode *); // also accepts AstForest*

  // For declaring & defining global variables and functions
  void declareGlobal(const String &name);
  void defineGlobal(const String &name,LambdaObject *object);
  ForeignFunctionObject *defineFunction(ForeignFunction,
					const String &name,int arity);

  // For looking up a built-in function or object
  LambdaObject *lookupGlobal(const String &);
  LexicalAddress getAddress(const String &);
  void assignToGlobal(const String &name,LambdaObject *value);

  // For deleting objects no longer needed
  void setGCthreshold(int);
  void registerWithGC(Garbage *);
  void unregisterGarbage(Garbage *);
  void runGC();
  void checkGarbageLevel();

  // For building expressions
  AstCombination *makeCombination(AstNode *function,
     AstForest *args,bool registerWithGC/*=true*/);
  AstCombination *makeComboWithArity(AstNode *function,
     int arity,bool registerWithGC/*=true*/);
  void setArg(AstCombination *,int argNum,AstNode *arg);
  AstLambda *makeLambda(int arity,AstForest *body,
     bool registerWithGC/*=true*/);
  AstForest *makeForest(bool registerWithGC/*=true*/);
  void appendTree(AstForest *,AstNode *);
  AstIntLit *makeIntLit(int,bool registerWithGC/*=true*/);
  AstFloatLit *makeFloatLit(float,bool registerWithGC/*=true*/);
  AstCharLit *makeCharLit(char,bool registerWithGC/*=true*/);
  AstStringLit *makeStringLit(const String &,
     bool registerWithGC/*=true*/);
  AstSymbol *makeSymbolNode(const String &,
     bool registerWithGC/*=true*/);
  AstBoundVariable *makeBoundVariable(LexicalAddress,
     bool registerWithGC/*=true*/);
  AstBoundVariable *makeGlobalRef(const String &globalIdent,
     int lexicalDepth,bool registerWithGC/*=true*/);

  // For printing out lambda source code
  void print(AstNode *,ostream &);

  // For allocating Lambda objects
  LambdaInt *makeInt(int,bool registerWithGC);
  LambdaFloat *makeFloat(float,bool registerWithGC);
  LambdaChar *makeChar(char,bool registerWithGC);
  LambdaString *makeString(const String &,bool registerWithGC);
  void makeImmortal(Garbage *);

  // For dealing with data types
  bool isNumeric(LambdaObject *); // true for int or float
  float asFloat(LambdaObject *); // must be int or float

  // For examining expressions
  AST_NodeType getNodeType(AstNode *);
  AstNode *getFunction(AstCombination *);
  AstNode *getArg(AstCombination *,int argNum);
  int numArgs(AstCombination *);
  int arity(AstLambda *);
  AstForest *getBody(AstLambda *);
  int numTrees(AstForest *);
  AstNode *getTree(AstForest *,int treeNum);

  // For modifying expressions
  AstNode *changeArg(AstCombination *,int argNum,
     AstNode *newArg);//returns the old arg
  AstNode *snipArg(AstCombination *,int argNum);
  AstNode *changeFunction(AstCombination *,AstNode *newFunction);
  AstForest *changeBody(AstLambda *,AstForest *newBody);
  AstNode *changeTree(AstForest *,int treeNum,AstNode *newTree);
  AstNode *snipTree(AstForest *,int treeNum,
     bool registerWithGC/*=false*/);
  void insertTree(AstForest *,int treeNum,AstNode *newTree);

  // For doing more sophisticated things
  RunTimeEnvironment &getEnvironment();
  Evaluator &getEvaluator();
  unsigned getRandomSeed() const;
  GarbageCollector &getGC();
  void registerMarkHook(MarkHook *h) {env.registerMarkHook(h);}
};
}

#endif
