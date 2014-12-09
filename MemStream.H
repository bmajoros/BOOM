/***********************************************************************
 MemStream.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_MemStream_H
#define INCL_BOOM_MemStream_H
#include "String.H"

namespace BOOM {
class MemStream
{
public:
  MemStream(const char *data);
  BOOM::String readLine();
  bool eof();
private:
  const char *p;
};
}

#endif
