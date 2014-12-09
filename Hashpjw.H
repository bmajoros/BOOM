/***********************************************************************
 Hashpjw.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 hashpjw algorithm is due to P.J. Weinberg
 ***********************************************************************/
#ifndef INCL_BOOM_Hashpjw_H
#define INCL_BOOM_Hashpjw_H
#include "String.H"
using namespace std;

namespace BOOM {
unsigned Hashpjw(BOOM::String,unsigned tableSize);

unsigned Hashpjw(const char *,unsigned tableSize);

//unsigned Hashpjw(const BOOM::String &,unsigned tableSize);
}

#endif
