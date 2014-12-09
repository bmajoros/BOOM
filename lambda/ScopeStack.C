/*
 ScopeStack.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "ScopeStack.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


ScopeStack::ScopeStack(int hashTableSize)
  : hashTableSize(hashTableSize)
{
  // ctor
}



void ScopeStack::replaceGlobal(AbstractSymbolNode *victim,
			       AbstractSymbolNode *newNode,
			       const char *key)
{
  getGlobalScope()->replace(victim,newNode,key);
}



AbstractSymbolNode *ScopeStack::findGlobal(const char *key)
{
  HashSymbolTable *table=getGlobalScope();
  return table->Find(key);
}



HashSymbolTable *ScopeStack::getGlobalScope()
{
  return (HashSymbolTable*) TheStack.bottom();
}




void ScopeStack::insertGlobal(AbstractSymbolNode *t,
			      const char *key)
{
  getGlobalScope()->Insert(t,key);
}




int ScopeStack::getGlobalScopeSize()
{
  return getGlobalScope()->GetNumEntries();
}




AbstractSymbolNode *ScopeStack::Find(const char *Key,
				     int &LexicalDepth)
{
  /*
   Searches the scope stack from top to bottom
   for the first instance of a symbol matchin
   Key.  If found, LexicalDepth is set to
   the lexical depth of the symbol, where symbols
   in the top symbol table have lexical depth of
   0.  If not found, LexicalDepth is set to the
   number of symbol tables in the stack.
  */

  LexicalDepth=0;
  int i, n=TheStack.size();
  for(i=n-1 ; i>=0 ; --i)
    {
      HashSymbolTable *symtab=
	(HashSymbolTable*) TheStack.asVector()[i];
      AbstractSymbolNode *tp=symtab->Find(Key);
      if(tp) return tp;
      ++LexicalDepth;
    }
  return 0;
}




void ScopeStack::Delete(const char *Key)
{
  // Deletes the innermost declaration of Key.
 
  int i, n=TheStack.size();
  for(i=n-1 ; i>=0 ; --i)
    {
      HashSymbolTable *symtab=(HashSymbolTable *)
	TheStack.asVector()[i];
      if(symtab->Find(Key))
	{
	  symtab->Delete(Key);
	  break;
	}
    }
  
}




void ScopeStack::Insert(AbstractSymbolNode *t,const char *Key)
{
  HashSymbolTable *table=(HashSymbolTable *) TheStack.top();
  table->Insert(t,Key);
}




int ScopeStack::GetCurrentScopeSize() //const
{
  HashSymbolTable *table=(HashSymbolTable *) TheStack.top();
  return table->GetNumEntries();
}





void ScopeStack::EnterScope()
{
  TheStack.push(new HashSymbolTable(hashTableSize));
}




void ScopeStack::LeaveScope()
{
  delete TheStack.pop();
}

