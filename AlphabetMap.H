/****************************************************************
 AlphabetMap.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_AlphabetMap_H
#define INCL_AlphabetMap_H
#include <iostream>
#include "Alphabet.H"
#include "Array1D.H"
using namespace std;

namespace BOOM {

/****************************************************************
 AlphabetMap : generic class for mapping symbols from one 
               alphabet to another, in constant time
 ****************************************************************/
class AlphabetMap
{
public:
  AlphabetMap() {}
  AlphabetMap(const Alphabet &domain,const Alphabet &range);
  AlphabetMap(int domainSize,int rangeSize);
  AlphabetMap(const AlphabetMap &);
  Symbol operator()(Symbol a) const {return map(a);}
  Symbol map(Symbol) const;
  Symbol unmap(Symbol) const;
  void defineMapping(Symbol from,Symbol to);
  void defineInverse(Symbol from,Symbol to);
  int getDomainSize() const;
  int getRangeSize() const;
  const Alphabet *getDomain();
  const Alphabet *getRange();
protected:
  const Alphabet *domain, *range;
  Array1D<BOOM::Symbol> theMap, inverseMap;
  void clear();
};



/****************************************************************
 AlphabetIdentityMap : maps any alphabet to itself via identity
 ****************************************************************/
class AlphabetIdentityMap : public AlphabetMap
{
public:
  AlphabetIdentityMap(const Alphabet &);
};



/****************************************************************
 DropGapMapping : maps ACGT->ACGT; anything else is ignored
 ****************************************************************/
class DropGapMapping : public AlphabetMap // ### was DropNMapping
{
public:
  DropGapMapping(const Alphabet &from,const Alphabet &to);
};

typedef DropGapMapping DropNMapping;


}

#endif

