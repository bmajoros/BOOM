/***********************************************************************
 MemStream.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "MemStream.H"
using namespace std;



BOOM::MemStream::MemStream(const char *p)
  : p(p)
{
}



BOOM::String BOOM::MemStream::readLine()
{
  const char *q=p;
  while(true)
    switch(*q)
      {
      case '\n': 
      case '\0': 
	goto end;
      default:
	++q;
      }
 end:
  const char *oldP=p;
  unsigned length=q-p;
  p=q+1;
  return BOOM::String(oldP,length);
}



bool BOOM::MemStream::eof()
{
  return *p=='\0';
}
