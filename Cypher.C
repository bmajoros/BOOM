/***********************************************************************
 Cypher.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "Cypher.H"
#include <iostream>
using namespace BOOM;

Cypher::Cypher(const BOOM::String &key)
  : key(key)
{
  // ctor
}


Cypher::Cypher(const char *p,int len)
{
  // ctor

  for(int i=0 ; i<len ; ++i, ++p) 
    key+=*p; 
}


BOOM::String Cypher::operator()(const BOOM::String &s)
{
  int n=s.length(), k=key.length();
  BOOM::String t;
  for(int i=0 ; i<n ; ++i)
    t+=s[i]^key[i%k];
  return t;
}


BOOM::String Cypher::f(const char *p,int len)
{
  int k=key.length();
  BOOM::String t;
  for(int i=0 ; i<len ; ++i, ++p)
    t+=*p^key[i%k];
  return t;
}



