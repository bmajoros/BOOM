/****************************************************************
 GapPatternAlphaMap.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GapPatternAlphaMap.H"
#include "DnaDashDotAlphabet.H"
#include "GapPatternAlphabet.H"
using namespace std;
using namespace BOOM;



GapPatternAlphaMap::GapPatternAlphaMap()
  : AlphabetMap(DnaDashDotAlphabet::global(),GapPatternAlphabet::global())
{
  DnaDashDotAlphabet &dashDotAlpha=DnaDashDotAlphabet::global();
  GapPatternAlphabet &gapPatternAlpha=GapPatternAlphabet::global();

  Symbol fA=dashDotAlpha.lookup('A');
  Symbol fC=dashDotAlpha.lookup('C');
  Symbol fG=dashDotAlpha.lookup('G');
  Symbol fT=dashDotAlpha.lookup('T');
  Symbol fN=dashDotAlpha.lookup('N');
  Symbol fDash=dashDotAlpha.lookup('-');
  Symbol fDot=dashDotAlpha.lookup('.');

  Symbol tN=gapPatternAlpha.lookup('N');
  Symbol tDash=gapPatternAlpha.lookup('-');
  Symbol tDot=gapPatternAlpha.lookup('.');
  defineMapping(fA,tN);
  defineMapping(fC,tN);
  defineMapping(fG,tN);
  defineMapping(fT,tN);
  defineMapping(fN,tN);
  defineMapping(fDash,tDash);
  defineMapping(fDot,tDot);

  defineInverse(tDot,fDot);
  defineInverse(tDash,fDash);
  defineInverse(tN,fN);
}



GapPatternAlphaMap &GapPatternAlphaMap::global()
{
  static GapPatternAlphaMap m;
  return m;
}

