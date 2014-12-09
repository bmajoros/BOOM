/***********************************************************************
 ApproximateAlignment.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "ApproximateAlignment.H"
#include <iostream>
#include "DnaAlphabet.H"
#include "AminoAlphabet.H"
#include "ProteinTrans.H"
#include <fstream>
using namespace BOOM;


/****************************************************************
                     class ApproxAlignCell
 */
ApproxAlignCell::ApproxAlignCell()
{
  // default ctor
}



ApproxAlignCell::ApproxAlignCell(int x,int y,MatchType matchType,
				 float matches,int length)
  : x(x), y(y), matchType(matchType), matches(matches), length(length)
{
  // ctor
}



void ApproxAlignCell::setCumulatives(float matches,int length)
{
  this->matches=matches;
  this->length=length;
}







