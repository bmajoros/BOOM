/*
 SymbolTable.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_SymbolTable_H
#define INCL_SymbolTable_H
#include "HashTable.H"
using namespace std;
using namespace BOOM;
namespace Lambda {

const int HASH_TABLE_SIZE=499;

// *******************************************
//	    class AbstractSymbolNode
// *******************************************
class AbstractSymbolNode
{
public:
  AbstractSymbolNode() {}
  virtual ~AbstractSymbolNode() {}
  virtual bool Equals(const char *Key)=0;
  AbstractSymbolNode *next;
};



// *******************************************
//	    class HashSymbolTable
// *******************************************

class HashSymbolTable
{
  SimpleHashTable<AbstractSymbolNode,HASH_TABLE_SIZE> hashTable;
  int NumEntries;
public:
  HashSymbolTable(int tableSize) ;
  AbstractSymbolNode *Find(const char *Key);
  void Insert(AbstractSymbolNode *t,const char *Key);
  void Delete(const char *Key);
  void replace(AbstractSymbolNode *victim,
	       AbstractSymbolNode *newNode,const char *key);
  int GetNumEntries() const;
};

}

#endif
