/****************************************************************
 TempFilename.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "TempFilename.H"
#include "Environment.H"
#include "Process.H"
#include "Time.H"
#include "Random.H"
#include "Constants.H"
using namespace std;
using namespace BOOM;


String TempFilename::get()
{
  String dir=Environment::lookup("TMPDIR");
  if(dir.isEmpty()) throw "Please set environment variable TMPDIR";
  if(dir.lastChar()=='/') dir.chop();
  const pid_t pid=Process::pid();
  const unsigned long lpid=static_cast<unsigned long>(pid);
  String date=getDateAndTime();
  date.removeWhitespace();
  int r=RandomNumber(0,INT_MAX-2);
  int tries=0, MAX_TRIES=1000000;
  while(1) {
    String filename=dir+'/'+lpid+"_"+date+"_"+r+".temp";
    int fd=open(filename.c_str(), O_CREAT | O_EXCL,S_IRWXU);
    if(fd!=-1) {
      close(fd);
      return filename;
    }
    r=RandomNumber(0,INT_MAX-2);
    if(++tries>MAX_TRIES) throw "Max tries exceeded in TempFilename:;get()";
  }
}


