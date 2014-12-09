/***********************************************************************
 Pipe.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Pipe_H
#define INCL_BOOM_Pipe_H
#include "File.H"
using namespace std;

namespace BOOM {
class Pipe : public BOOM::File
{
public:
  Pipe(const BOOM::String &command,const BOOM::String &mode);
  virtual void close();
};
}

#endif
