/***********************************************************************
 IndexedFasta.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "IndexedFasta.H"
#include <iostream>


BOOM::IndexedFasta::IndexedFasta(const BOOM::String &filename)
  : file(filename)
{
  // ctor
}



void BOOM::IndexedFasta::load(long begin,long len,char *buffer)
{
  file.seek(begin);
  file.read(len,(void*)buffer);
}



BOOM::String *BOOM::IndexedFasta::load(long begin,long len)
{
  char *buffer=new char[len+1];
  load(begin,len,buffer);
  buffer[len]='\0';
  BOOM::String *s=new BOOM::String(buffer);
  delete [] buffer;
  return s;
}



void BOOM::IndexedFasta::load(long begin,long len,BOOM::String &s)
{
  char *buffer=new char[len+1];
  load(begin,len,buffer);
  buffer[len]='\0';
  s=buffer;
  delete [] buffer;
}



long BOOM::IndexedFasta::getSize() const
{
  return file.getSize();
}


