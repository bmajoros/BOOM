/***********************************************************************
 Environment.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "Environment.H"
#include <iostream>
#include <stdlib.h>


BOOM::String BOOM::Environment::lookup(const BOOM::String &name)
{
  return BOOM::String(getenv(name.c_str()));
}


