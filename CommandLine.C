/***********************************************************************
 CommandLine.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <unistd.h>
#include <stdio.h>
#include "CommandLine.H"
using namespace std;



BOOM::CommandLine::CommandLine(int argc,char *argv[],const char *options)
  : argc(argc), argv(&argv[0])
{
  loopThroughOptions(options);
}



BOOM::String BOOM::CommandLine::arg(int i)
{
  return args[i];
}



BOOM::String BOOM::CommandLine::optParm(char c)
{
  //if(optionParm.find(c)==optionParm.end())
  if(!optionParm.isDefined(c))
    return BOOM::String("");
  return optionParm[c];
}



bool BOOM::CommandLine::option(char c)
{
  return usedOption.find(c)==usedOption.end() ? false : usedOption[c];
}



bool BOOM::CommandLine::takesParameter(char c,const char *options)
{
  for(int i=0 ; options[i] ; ++i)
    if(options[i]==c)
      return options[i+1]==':';
  return false;
}



int BOOM::CommandLine::numArgs()
{
  return args.size();
}



void BOOM::CommandLine::loopThroughOptions(const char *options)
{
  int c;
  while((c=getopt(argc,argv,options))!=EOF)
    {
      usedOption[c]=true;
      if(takesParameter(c,options))
	optionParm[c]=BOOM::String(optarg);
    }

  for(int i=optind ; i<argc ; ++i)
    args.push_back(argv[i]);
}
