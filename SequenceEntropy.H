/***********************************************************************
 SequenceEntropy.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_SequenceEntropy_H
#define INCL_BOOM_SequenceEntropy_H
#include "Sequence.H"
#include "String.H"

namespace BOOM {
class SequenceEntropy
{
public:
  static double entropy(const Sequence &,double &maxEntropy);
  static double entropy(const BOOM::String &,double &maxEntropy);
  static double jointEntropy(const Sequence &,int order,double &maxEntropy);
  static double jointEntropy(const BOOM::String &,int order,
			     double &maxEntropy);
  static double conditionalEntropy(const Sequence &,int order);
  static double conditionalEntropy(const BOOM::String &,int order);
};
}

#endif
