/***********************************************************************
 Environment.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Environment_H
#define INCL_Environment_H
using namespace std;
#include "String.H"

namespace BOOM {
class Environment
{
public:
  static BOOM::String lookup(const BOOM::String &);
};
}

#endif
