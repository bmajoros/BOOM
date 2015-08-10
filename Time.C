/***********************************************************************
 Time.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "Time.H"
using namespace std;



BOOM::String BOOM::getDateAndTime()
{
  time_t t=time(NULL);
  return BOOM::String(ctime(&t));
}



BOOM::String BOOM::Time::elapsedTime()
{
  // seconds
  float sec=elapsedSeconds();
  if(sec<60)
    return BOOM::String("")+sec+" sec";

  // minutes
  float min=sec/60;
  if(min<60)
    return BOOM::String("")+min+" min";

  // hours
  float hours=min/60;
  if(hours<24)
    return BOOM::String("")+hours+" hours";
  
  // days
  float days=hours/24;
  return BOOM::String("")+days+" days";
}



float BOOM::Time::elapsedSeconds()
{
  gettimeofday(&timevalStop,NULL);
  float seconds=timevalStop.tv_sec-timevalStart.tv_sec;
  float uSec=timevalStop.tv_usec-timevalStart.tv_usec;
  
  return seconds+uSec/1000000.0;
}



void BOOM::Time::startCounting()
{
  gettimeofday(&timevalStart,NULL);
}



void BOOM::Time::stopCounting()
{
  gettimeofday(&timevalStop,NULL);
}
