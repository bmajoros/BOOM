/***********************************************************************
 MolecularSequenceType.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_MolecularSequenceType_H
#define INCL_MolecularSequenceType_H
#include <iostream>
#include "String.H"

namespace BOOM {

enum MolecularSequenceType
  {
    DNA,
    PROTEIN
  };


MolecularSequenceType seqTypeFromString(const BOOM::String &);
BOOM::String seqTypeToString(MolecularSequenceType);
ostream &operator<<(ostream &os,BOOM::MolecularSequenceType);
istream &operator>>(istream &is,BOOM::MolecularSequenceType &);

}

#endif
