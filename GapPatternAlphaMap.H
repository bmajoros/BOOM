/****************************************************************
 GapPatternAlphaMap.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GapPatternAlphaMap_H
#define INCL_GapPatternAlphaMap_H
#include <iostream>
#include "AlphabetMap.H"
using namespace std;

namespace BOOM {

/******************************************************************
 GapPatternAlphaMap : maps DnaDashDotAlphabet to GapPatternAlphabet
 ******************************************************************/
class GapPatternAlphaMap : public AlphabetMap
{
public:
  GapPatternAlphaMap();
  static GapPatternAlphaMap &global();
};

}

#endif

