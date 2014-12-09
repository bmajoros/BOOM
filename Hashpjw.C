/***********************************************************************
 Hashpjw.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Hashpjw.H"
using namespace std;



unsigned BOOM::Hashpjw(const char *s,unsigned tableSize)
{
    const BOOM::String ss=s;
    return BOOM::Hashpjw(ss,tableSize);
}


/*
unsigned BOOM::Hashpjw(const BOOM::String &s,unsigned tableSize)
{
  return BOOM::Hashpjw(BOOM::String(s),tableSize);
}
*/


unsigned BOOM::Hashpjw(BOOM::String s,unsigned tableSize)
{
  unsigned length=s.length();
  if(length<5) 
    {
      s=s+s;
      length*=2;
    }
  int h=0;
  for(int i=0 ; i<length ; ++i)
    {
      h=(h<<4)+s[i];
      int g=h & 0xf000;
      if(g) h=h^(g>>8);
    }
  return h % tableSize;
}
