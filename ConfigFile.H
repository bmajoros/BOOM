/***********************************************************************
 ConfigFile.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_ConfigFile_H
#define INCL_BOOM_ConfigFile_H
#include <iostream>
#include "StrTokenizer.H"
#include "Map.H"
using namespace std;

namespace BOOM {
class ConfigFile
{
public:
  ConfigFile(BOOM::String fname);
  virtual BOOM::String lookup(BOOM::String attr);
  virtual BOOM::String lookupOrDie(BOOM::String attr);
  virtual double getDoubleOrDie(BOOM::String attr);
  virtual float getFloatOrDie(BOOM::String attr);
  virtual int getIntOrDie(BOOM::String attr);
  virtual long getLongOrDie(BOOM::String attr);
  virtual bool getBoolOrDie(BOOM::String attr);
  virtual bool isDefined(BOOM::String key);
  void enter(BOOM::String,BOOM::String);
  BOOM::Map<BOOM::String,BOOM::String> &peek();
protected:
  ConfigFile();
private:
  BOOM::Map<BOOM::String,BOOM::String> dict;
  const char *charLookupOrDie(BOOM::String attr);
  void load(BOOM::String fname);
  void processLine(BOOM::StrTokenizer &);
};
}

#endif
