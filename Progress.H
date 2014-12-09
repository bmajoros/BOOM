/***********************************************************************
 Progress.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Progress_H
#define INCL_BOOM_Progress_H
#include "String.H"
using namespace std;

namespace BOOM {
class Progress
{
public:
  Progress();
  BOOM::String getProgress(unsigned long workDone);
  void start(unsigned long totalWork);
private:
  unsigned long totalWork;
  long startTime;
};
}

#endif
