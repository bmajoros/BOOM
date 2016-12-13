/***********************************************************************
 Alphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "Alphabet.H"
#include <iostream>
#include "String.H"
#include <string.h>

BOOM::Alphabet::Alphabet(const char *initializer) : numElements(0)
{
  for(int i=0 ; i<256 ; ++i)
    charToInt[i]=intToChar[i]=INVALID_SYMBOL;

  if(initializer)
    {
      int n=strlen(initializer);
      for(int i=0 ; i<n ; ++i) add(initializer[i]);
    }
}



int BOOM::Alphabet::getNumElements() const
{
  return numElements;
}



int BOOM::Alphabet::add(char c)
{
  charToInt[static_cast<int>(c)]=static_cast<char>(numElements);
  intToChar[numElements]=c;
  return numElements++;
}



ostream &operator<<(ostream &os,BOOM::Alphabet &alphabet)
{
  alphabet.printOn(os);
  return os;
}



bool BOOM::Alphabet::load(istream &is)
{
  char buffer[260];
  is.getline(buffer,260);
  if(is.good())
    {
      char *p=static_cast<char*>(buffer);
      while(*p) add(*p++);
      return true;
    }
  return false;
}



bool BOOM::Alphabet::save(ostream &os)
{
  for(int i=0 ; i<numElements ; ++i) os << intToChar[i];
  os << endl;
  return os.good() ? true : false;
}



Symbol BOOM::Alphabet::complement(Symbol s) const
{
  return lookup(complement(lookup(s)));
}



char BOOM::Alphabet::complement(char c) const
{
  switch(c)
    {
    case 'A': return 'T';
    case 'G': return 'C';
    case 'C': return 'G';
    case 'T': return 'A';
    case 'N': return 'N';
    }
  throw 
    BOOM::String("BOOM::Alphabet::complement:  can't complement base '")+
    c+"', ASCII "+int(c);
}



void BOOM::Alphabet::printOn(ostream &os)
{
  for(int i=0 ; i<numElements ; ++i) os << intToChar[i] << ',';
}



bool BOOM::Alphabet::isDefined(char c) const
{
  return lookup(c)!=INVALID_SYMBOL;
}

