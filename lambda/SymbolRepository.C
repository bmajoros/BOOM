/*
 SymbolRepository.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "SymbolRepository.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;

// **************************************************
//               SymRepNode methods
// **************************************************

SymRepNode::SymRepNode(const String &lexeme)
  : lexeme(lexeme), symbol(NULL)
{
  // ctor
}



void SymRepNode::setSymbol(Symbol *symbol)
{
  this->symbol=symbol;
}



Symbol *SymRepNode::getSymbol()
{
  return symbol;
}



const String &SymRepNode::getLexeme()
{
  return lexeme;
}



bool SymRepNode::Equals(const char *Key)
{
  return !strcmp(Key,lexeme.c_str());
}




// **************************************************
//              SymbolRepository methods
// **************************************************

SymbolRepository::SymbolRepository() : table(41)
{
  // ctor
}



Symbol *SymbolRepository::findOrCreate(const String &lexeme)
{
  SymRepNode *node=(SymRepNode *)
    table.Find(lexeme.c_str());
  if(node)
    return node->getSymbol();
  else
    {
      node=new SymRepNode(lexeme);
      Symbol *symbol=new Symbol(node->getLexeme());
      node->setSymbol(symbol);
      table.Insert(node,lexeme.c_str());
      return symbol;
    }
}





