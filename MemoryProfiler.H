/***********************************************************************
 MemoryProfiler.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_MemoryProfiler_H
#define INCL_MemoryProfiler_H
using namespace std;
#include "String.H"
#include <iostream>

namespace BOOM {
class MemoryProfiler
{
public:
  static void report(const BOOM::String &label,ostream & =cout);
};
}

#endif
