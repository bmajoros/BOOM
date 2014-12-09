/***********************************************************************
 MolecularSequenceType.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/

#include "MolecularSequenceType.H"

namespace BOOM {


MolecularSequenceType seqTypeFromString(const BOOM::String &s) {
  if(s=="DNA") return DNA;
  if(s=="PROTEIN") return PROTEIN;
  throw String("Invalid MolecularSequenceType in seqTypeFromString() : ")+s;
}



BOOM::String seqTypeToString(MolecularSequenceType t) {
  switch(t)
    {
    case DNA: return "DNA";
    case PROTEIN: return "PROTEIN";
    default: throw String("Unknown sequence type: ")+t;
    }

}



ostream &operator<<(ostream &os,BOOM::MolecularSequenceType t) {
  os<<seqTypeToString(t);
  return os;
}



istream &operator>>(istream &is,BOOM::MolecularSequenceType &t) {
  String s;
  is>>s;
  t=seqTypeFromString(s);
  return is;
}



}







