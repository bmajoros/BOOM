/*
 LambdaCmdLine.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaCmdLine_H
#define INCL_LambdaCmdLine_H

#include "BOOM/String.H"
#include "BOOM/ElasVector.H"

using namespace std;
using namespace BOOM;

namespace Lambda {
class LambdaCmdLine
{
  bool quietGC; //  -quietgc
  ElasVector<String*> filenames; // ### need to add code to delete these
  int numFilenames;
  int argc;
  char **argv;

  void processArgs(int argc,char *argv[]);
  int processArg(int i);
  int processSwitch(int i);
  void processFilename(const char *arg);
public:
  LambdaCmdLine(int argc,char *argv[]);
  virtual ~LambdaCmdLine();
  bool isGCquiet() const;
  int getNumFilenames() const;
  const String &getIthFilename(int i);
};
}
#endif
