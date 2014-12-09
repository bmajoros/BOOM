/*
 SymbolRepository.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_SymbolRepository_H
#define INCL_SymbolRepository_H
#include "SymbolTable.H"
#include "Symbol.H"
using namespace std;
using namespace BOOM;
namespace Lambda {

// **************************************************
//              class SymRepNode
// **************************************************

// Don't use this directly.  It is the type of node
// stored in the HashSymbolTable<> in the
// SymbolRepository.

class SymRepNode : public AbstractSymbolNode
{
  String lexeme;
  Symbol *symbol;
public:
  SymRepNode(const String &lexeme);
  void setSymbol(Symbol *);
  Symbol *getSymbol();
  const String &getLexeme();
  virtual bool Equals(const char *Key);
};



// **************************************************
//             class SymbolRepository
// **************************************************

// A place to store LAMBDA symbols (like 'x or 'y)

class SymbolRepository
{
  HashSymbolTable table; // size=41
public:
  SymbolRepository();
  Symbol *findOrCreate(const String &lexeme);
};

}

#endif
