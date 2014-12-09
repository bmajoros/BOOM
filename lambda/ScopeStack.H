/*
 ScopeStack.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_SCOPESTK_H
#define INCL_SCOPESTK_H
#include "SymbolTable.H"
#include "BOOM/Stack.H"
using namespace std;
using namespace BOOM;
namespace Lambda {

class STACK_UNDERFLOW
{
};

class ScopeStack 
{
  Stack<HashSymbolTable*> TheStack;
  int hashTableSize;

  HashSymbolTable *getGlobalScope();
public:
  ScopeStack(int hashTableSize);
  AbstractSymbolNode *Find(const char *Key,int &LexicalDepth);
  void Insert(AbstractSymbolNode *t,const char *Key); // throws STACK_UNDERFLOW if stack empty
  void Delete(const char *Key);
  void replaceGlobal(AbstractSymbolNode *victim,
		     AbstractSymbolNode *newNode,const char *key);
  void EnterScope();
  int GetCurrentScopeSize(); // throws STACK_UNDERFLOW if stack is empty
  void LeaveScope(); // throws STACK_UNDERFLOW if stack is empty

  void insertGlobal(AbstractSymbolNode *,const char *);
  AbstractSymbolNode *findGlobal(const char *key);
  int getGlobalScopeSize();
};

}

#endif
