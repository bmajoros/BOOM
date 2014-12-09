/***********************************************************************
 CommandLine.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_CommandLine_H
#define INCL_BOOM_CommandLine_H
#include "Vector.H"
#include "String.H"
#include "Map.H"
using namespace std;

namespace BOOM {
class CommandLine
{
public:
  CommandLine(int argc,char *argv[],const char *options);
  BOOM::String arg(int);
  BOOM::String optParm(char);
  bool option(char);
  int numArgs();
private:
  int argc;
  char **argv;
  BOOM::Vector<BOOM::String> args;
  BOOM::Map<char,bool> usedOption;
  BOOM::Map<char,BOOM::String> optionParm;
  bool takesParameter(char c,const char *options);
  void loopThroughOptions(const char *options);
};
}

#endif
