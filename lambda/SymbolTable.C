/*
 SymbolTable.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "SymbolTable.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


HashSymbolTable::HashSymbolTable(int tableSize) 
  : NumEntries(0)
{
}



int HashSymbolTable::GetNumEntries() const 
{ 
  return NumEntries; 
}



AbstractSymbolNode *HashSymbolTable::Find(const char *Key)
{
  int hashValue=hashTable.HashString(Key);
  AbstractSymbolNode *t=
    (AbstractSymbolNode *) hashTable.GetFirstAt(hashValue);
  while(t)
    {
      if(t->Equals(Key)) return t;
      t=(AbstractSymbolNode*)(t->next);
    }
  return t;
}



void HashSymbolTable::Delete(const char *Key)
{
  int hashValue=hashTable.HashString(Key);
  AbstractSymbolNode *t=Find(Key);
  if(t) hashTable.Delete(t,hashValue);
}



void HashSymbolTable::Insert(AbstractSymbolNode *t,
			     const char *Key)
{
  int hashValue=hashTable.HashString(Key);
  hashTable.Insert(t,hashValue);
  ++NumEntries;
}



void HashSymbolTable::replace(AbstractSymbolNode *victim,
			      AbstractSymbolNode *newNode,
			      const char *key)
{
  const int hashValue=hashTable.HashString(key);
  hashTable.Delete(victim,hashValue);
  hashTable.Insert(newNode,hashValue);
}
