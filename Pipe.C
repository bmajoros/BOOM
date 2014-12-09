/***********************************************************************
 Pipe.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Pipe.H"
using namespace std;



BOOM::Pipe::Pipe(const BOOM::String &command,const BOOM::String &mode)
  : BOOM::File()
{
  fp=popen(command.c_str(),mode.c_str());
  if(!fp) 
    throw BOOM::String("Can't open pipe for command \"")+
      command+"\"";
  filename=command;
  this->mode=mode;
}



void BOOM::Pipe::close()
{
  if(fp!=NULL)
    pclose(fp);
  fp=NULL;
}
