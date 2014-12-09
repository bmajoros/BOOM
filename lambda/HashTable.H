/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
// ================================
// HashTable.H :  intrusive hash table
//
// Ported to Solaris on 3/27/96
// Updated for Lambda on 8/28/08
// ================================

#ifndef INCL_BoomHashTable_H
#define INCL_BoomHashTable_H
#include "BOOM/Exceptions.H"
#include "BOOM/Hashpjw.H"
using namespace std;
using namespace BOOM;
namespace Lambda {


// *******************************************
//	    class HashTableIterator<>
// *******************************************

template<class ElementType>
class HashTableIterator {
  int tableSize;
  ElementType **theTable;
  int currentHashBucket;
  ElementType *currentElement;
  void FindNext();
public:
  HashTableIterator(int tableSize,ElementType **theTable);
  void Reset();
  bool IsValid();
  ElementType *CurrentElement();
  void Advance();
};




// *******************************************
//	      class SimpleHashTable
// *******************************************

// This class gives very basic support for hash
// tables.  Type T must be derived from link_node
// (or have a "next" pointer which is public),
// and the TableSize should be a prime.

template <class T,int TableSize>
class SimpleHashTable {
  T *TheTable[TableSize];
public:
  SimpleHashTable();
  virtual ~SimpleHashTable();
  T *GetFirstAt(unsigned HashValue);
  const T *GetFirstAt(unsigned HashValue) const;
  void Insert(T *t,unsigned HashValue);
  void Delete(T *t,unsigned HashValue);
  void LetGoOf(T *t,unsigned HashValue);
  unsigned HashString(const char *p) const;
  HashTableIterator<T> *GetIterator();
};

}
using namespace Lambda;


// *******************************************
//	    SimpleHashTable<> methods
// *******************************************
template <class T,int TableSize>
HashTableIterator<T> *SimpleHashTable<T,TableSize>::GetIterator()
{
  return new HashTableIterator<T>(TableSize,TheTable);
}



template <class T,int TableSize>
T *SimpleHashTable<T,TableSize>::GetFirstAt(unsigned HashValue) 
{ 
  return TheTable[HashValue % TableSize]; 
}



template <class T,int TableSize>
const T *SimpleHashTable<T,TableSize>::GetFirstAt(unsigned HashValue) const
{ 
  return TheTable[HashValue % TableSize]; 
}



template <class T,int TableSize>
SimpleHashTable<T,TableSize>::SimpleHashTable()
{
  for(int i=0 ; i<TableSize ; i++)
    TheTable[i]=0;
}



template <class T,int TableSize>
SimpleHashTable<T,TableSize>::~SimpleHashTable()
{
  for(int i=0 ; i<TableSize ; i++)
    {
      // Delete the ith list by traversing it
      
      T *ThisNode=TheTable[i];
      while(ThisNode)
	{
	  //T *NextNode=dynamic_cast<T*>(ThisNode->next);
	  T *NextNode=(T*)(ThisNode->next);
	  delete ThisNode;
	  ThisNode=NextNode;
	}
    }
}



template <class T,int TableSize>
void SimpleHashTable<T,TableSize>::Insert(T *t,unsigned HashValue)
{
  t->next=TheTable[HashValue % TableSize];
  TheTable[HashValue % TableSize]=t;
}



template <class T,int TableSize>
void SimpleHashTable<T,TableSize>::Delete(T *t,unsigned HashValue)
{
  LetGoOf(t,HashValue);
  delete t;
}



template <class T,int TableSize>
void SimpleHashTable<T,TableSize>::LetGoOf(T *t,unsigned HashValue)
{
  if(TheTable[HashValue % TableSize]==t)
    TheTable[HashValue % TableSize]=(T*)(t->next);
  else
    {
      T *p=TheTable[HashValue % TableSize];
      while(p && p->next != t) 
	p=(T*)(p->next);
      if(p)
	p->next=t->next;
    }
}



template <class T,int TableSize>
unsigned SimpleHashTable<T,TableSize>::HashString(const char *s) const
{
  return Hashpjw(s,TableSize);
}





// *******************************************
//	  HashTableIterator<> methods
// *******************************************

template<class ElementType>
void HashTableIterator<ElementType>::FindNext()
{
  // This method ensures that currentElement points
  // to a non-NULL object by searching ahead if 
  // currentElement is currently NULL.  If no more
  // objects can be found, it is left as NULL.

  while(!currentElement)
    {
      if(currentHashBucket>=tableSize-1) return;
      ++currentHashBucket;
      currentElement=theTable[currentHashBucket];
    }
}



template<class ElementType>
HashTableIterator<ElementType>::HashTableIterator(int tableSize,
  ElementType **theTable)
  : theTable(theTable), tableSize(tableSize), currentElement(0)
{
  // ctor

  Reset();
}



template<class ElementType>
void HashTableIterator<ElementType>::Reset()
{
  currentHashBucket=-1;
  FindNext();
}



template<class ElementType>
bool HashTableIterator<ElementType>::IsValid()
{
  return currentElement!=NULL;
}



template<class ElementType>
ElementType *HashTableIterator<ElementType>::CurrentElement()
{
  return currentElement;
}



template<class ElementType>
void HashTableIterator<ElementType>::Advance()
{
  if(!IsValid())
    throw RootException("Cannot advance hash table iterator");

  currentElement=(ElementType*) currentElement->next;
  FindNext();
}




#endif
