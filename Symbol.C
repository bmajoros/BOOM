/***********************************************************************
 Symbol.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "Symbol.H"
#include <iostream>
#include "String.H"
using namespace BOOM;


Symbol::Symbol(int i) : alphabetIndex(i)
{
}



bool Symbol::operator==(const Symbol &other) 
{ 
  return alphabetIndex==other.alphabetIndex;
}



Symbol::operator int() const 
{ 
  return alphabetIndex;
}



int Symbol::getIndex() const
{
  return alphabetIndex;
}



Symbol &Symbol::operator++() 
{ 
  ++alphabetIndex; 
  return *this;
}



