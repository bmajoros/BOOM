/*
 RunTimeEnvironment.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_RunTimeEnvironment_H
#define INCL_RunTimeEnvironment_H
#include "SymbolRepository.H"
#include "ScopeStack.H"
#include "AST.H"
#include "RunTimeStack.H"
#include "LexicalAddress.H"
#include "GC.H"
using namespace std;
using namespace BOOM;
namespace Lambda {

class ConsCell;

// **************************************************
//              class LambdaSymTabNode
// **************************************************

// A fully-resolved identifier

class LambdaSymTabNode : public AbstractSymbolNode
{
  String lexeme;
  int lexicalPosition;
public:
  LambdaSymTabNode(const String &lexeme,int lexicalPosition);
  virtual bool Equals(const char *Key);
  int getLexicalPosition() const;
  virtual bool isResolved();
  const String &getLexeme() const;
};


// **************************************************
//              class BackpatchNode
// **************************************************

// An unresolved identifier needing backpatching
// (and having a list of nodes to backpatch)

class BackpatchNode : public LambdaSymTabNode
{
  AstForest patchList;
public:
  BackpatchNode(const String &lexeme,
		int lexicalPosition);
  void addToList(AstVariable *);
  AstForest &getPatchList();
  virtual bool isResolved();
};



// **************************************************
//            class RunTimeEnvironment
// **************************************************

class RunTimeEnvironment
{
  SymbolRepository &symbolRepository;
  ScopeStack &scopeStack;
  RunTimeStack &runTimeStack;
  GarbageCollector &gc;
  ElasVector<String*> &globalNames;
  Immortals &immortals;
  ConsCell *prepareList(Array1D<LambdaObject*> &parmValues,int startingAt);
public:
  // ctors & dtors
  RunTimeEnvironment();
  virtual ~RunTimeEnvironment();

  // Stack methods
  int getNumParms();
  LambdaObject *getParm(int);
  ActivationRecord *prepareAR(Array1D<LambdaObject*> &parmValues,
			      int arity,ActivationRecord *staticChain=NULL);
  LambdaObject *retrieve(LexicalAddress);
  LambdaObject *retrieveGlobal(int lexicalPosition);
  void set(LexicalAddress,LambdaObject *);
  void pushGlobalAR(); // after declaring but before defining globals
  void defineGlobal(const String &name,LambdaObject *object);
  ActivationRecord *getTopAR();
  ActivationRecord *getGlobalAR();
  void pushAR(ActivationRecord *);
  void popAR(); // & give it to the GC
  void popAR_unlessGlobal();
  void popDownTo(ActivationRecord *);
  void cleanStack();

  // Symbol methods
  Symbol *findOrCreateSymbol(const String &);

  // Scope methods
  LambdaSymTabNode *Find(const String &key,int &lexicalDepth);
  void assign(const String &variableName,LambdaObject *value);
  int GetCurrentScopeSize();
  int getGlobalScopeSize();
  void Declare(LambdaSymTabNode *,const String &key);
  void declareGlobal(const String &name);
  void declareGlobal(BackpatchNode *,const String &key);
  void resolveGlobal(BackpatchNode *,LambdaSymTabNode *);
  void backpatch(LambdaSymTabNode *symTabNode);
  void declareGlobalAndBackpatch(const String &name);
  void enterScope();
  void leaveScope();
  const String &getGlobalName(int lexicalPosition);

  // Garbage methods
  void registerGarbage(Garbage *);
  void unregisterGarbage(Garbage *);
  void runGC();
  inline void checkGarbageLevel() { if(gc.shouldCollect()) runGC(); }
  void registerMarkHook(MarkHook *);
  void makeImmortal(Garbage *);
  GarbageCollector &getGC();
  void setGCthreshold(int);

  // For debugging
  int getStackDepth();
};

/********************************************************
       NOTES ON USING THE RUN-TIME-ENVIRONMENT

1) Defining global objects must be done in the following
   order:
     a) declare all globals using declareGlobal()
     b) allocate global AR using pushGlobalAR()
     c) define all globals using defineGlobal()


********************************************************/


}

#endif
