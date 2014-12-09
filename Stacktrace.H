/***********************************************************************
 Stacktrace.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Stacktrace_H
#define INCL_BOOM_Stacktrace_H
#include "String.H"
using namespace std;


namespace BOOM {
class Crasher  
{
public:
  virtual void crash() {};
};



class Stacktrace  
{
public:
  Stacktrace(const char *message);
  Stacktrace(const BOOM::String &);
};
}

#endif
