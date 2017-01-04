/****************************************************************
 AlphabetMap.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "AlphabetMap.H"
#include "DnaAlphabet.H"
#include "PureDnaAlphabet.H"
using namespace std;
using namespace BOOM;


/****************************************************************
                      AlphabetMap methods
 ****************************************************************/

AlphabetMap::AlphabetMap(const Alphabet &from,const Alphabet &to)
  : theMap(from.getNumElements()),
    inverseMap(to.getNumElements()),
    domain(&from), range(&to)
{
  clear();
}



AlphabetMap::AlphabetMap(int domainSize,int rangeSize)
  : theMap(domainSize),
    inverseMap(rangeSize),
    domain(NULL), range(NULL)
{
  clear();
}



AlphabetMap::AlphabetMap(const AlphabetMap &other)
  : theMap(other.theMap),
    inverseMap(other.inverseMap),
    domain(other.domain),
    range(other.range)
{
  clear();
}



void AlphabetMap::clear() 
{
  theMap.setAllTo(INVALID_SYMBOL);
  inverseMap.setAllTo(INVALID_SYMBOL);
}



const Alphabet *AlphabetMap::getDomain() 
{
  return domain;
}



const Alphabet *AlphabetMap::getRange() 
{
  return range;
}



Symbol AlphabetMap::map(Symbol s) const
{
  return theMap[s];
}



Symbol AlphabetMap::unmap(Symbol s) const
{
  return inverseMap[s];
}



int AlphabetMap::getDomainSize() const
{
  return theMap.size();
}



int AlphabetMap::getRangeSize() const
{
  return inverseMap.size();
}



void AlphabetMap::defineMapping(Symbol from,Symbol to)
{
  theMap[from]=to;
}



void AlphabetMap::defineInverse(Symbol from,Symbol to)
{
  inverseMap[from]=to;
}



/****************************************************************
                   AlphabetIdentityMap methods
 ****************************************************************/

AlphabetIdentityMap::AlphabetIdentityMap(const Alphabet &a)
  : AlphabetMap(a,a)
{
  int n=a.getNumElements();
  for(Symbol i=0 ; i<n ; ++i)
    {
      defineMapping(i,i);
      defineInverse(i,i);
    }
}



/****************************************************************
                     DropGapMapping methods
 ****************************************************************/

DropGapMapping::DropGapMapping(const Alphabet &from,const Alphabet &to)
  : AlphabetMap(from,to)
{
  Symbol fA=domain->lookup('A');
  Symbol fC=domain->lookup('C');
  Symbol fG=domain->lookup('G');
  Symbol fT=domain->lookup('T');

  Symbol tA=range->lookup('A');
  Symbol tC=range->lookup('C');
  Symbol tG=range->lookup('G');
  Symbol tT=range->lookup('T');

  defineMapping(fA,tA);
  defineMapping(fC,tC);
  defineMapping(fG,tG);
  defineMapping(fT,tT);

  defineInverse(tA,fA);
  defineInverse(tC,fC);
  defineInverse(tG,fG);
  defineInverse(tT,fT);
}



