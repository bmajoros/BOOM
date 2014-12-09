/***********************************************************************
 IndexedFasta.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_IndexedFasta_H
#define INCL_BOOM_IndexedFasta_H
#include "String.H"
#include "File.H"

namespace BOOM {
class IndexedFasta
{
  BOOM::File file;
public:
  IndexedFasta(const BOOM::String &filename);
  void load(long begin,long len,char *);
  BOOM::String *load(long begin,long len);
  void load(long begin,long len,BOOM::String &);
  long getSize() const;
};
}

#endif
