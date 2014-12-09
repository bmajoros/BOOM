/*
 LambdaCmdLine.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "CommandLine.H"
#include <sstream>
#include "LambdaException.H"

using namespace std;
using namespace BOOM;
using namespace Lambda;

namespace Lambda {
extern int GC_THRESHOLD;
}

LambdaCmdLine::LambdaCmdLine(int argc,char *argv[])
  : quietGC(false), filenames(NULL), numFilenames(0),
    argc(argc), argv(argv)
{
  // ctor

  processArgs(argc,argv);
}



LambdaCmdLine::~LambdaCmdLine()
{
  // dtor

  int i;
  for(i=0 ; i<numFilenames ; ++i)
    delete filenames[i];
}



int LambdaCmdLine::processSwitch(int i)
{
  const char *arg=argv[i];
  ++arg; // pass the '-'

  if(!strcmp(arg,"quietgc"))
    {
      quietGC=true;
      return 1;
    }
  else if(!strcmp(arg,"help"))
    {
      cerr << "lambda [-quietgc] [-gc threshold] [filename(s)]" << endl;
      cerr << "   -quietgc turns off GC messages" << endl;
      cerr << "   -gc threshold specifies when to run the GC" << endl;
      exit(0);
    }
  else if(!strcmp(arg,"gc"))
    {
      if(i==argc-1) throw RootException("specify threshold after -gc");
      Lambda::GC_THRESHOLD=atoi(argv[i+1]);
      return 2;
    }
  else
    {
      ostringstream os;
      os << "Illegal switch: -" << arg;
      throw RootException(os.str());
    }
}



void LambdaCmdLine::processFilename(const char *arg)
{
  filenames[numFilenames]=new String(arg);
  ++numFilenames;
}



int LambdaCmdLine::processArg(int i)
{
  if(argv[i][0]=='-')
    return processSwitch(i);

  processFilename(argv[i]);
  return 1;
}



void LambdaCmdLine::processArgs(int argc,char *argv[])
{
  int i;
  for(i=1 ; i<argc ; )
    i+=processArg(i);
}



bool LambdaCmdLine::isGCquiet() const
{
  return quietGC;
}



int LambdaCmdLine::getNumFilenames() const
{
  return numFilenames;
}



const String &LambdaCmdLine::getIthFilename(int i)
{
  return (String &) *filenames[i];
}





