/***********************************************************************
 Time.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Time_H
#define INCL_BOOM_Time_H
#include <sys/time.h>
#include "String.H"
using namespace std;
namespace BOOM {

BOOM::String getDateAndTime();


class Time
{
public:
  BOOM::String elapsedTime();
  float elapsedSeconds();
  void startCounting();
  void stopCounting();
private:
  timeval timevalStart, timevalStop;
};
}

#endif
