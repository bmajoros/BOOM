/***********************************************************************
 Cypher.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_Cypher_H
#define INCL_Cypher_H
#include "String.H"

namespace BOOM {
class Cypher
{
  BOOM::String key;
public:
  Cypher(const BOOM::String &key);
  Cypher(const char *key,int len);
  BOOM::String operator()(const BOOM::String &);
  BOOM::String f(const char *p,int len);
  BOOM::String operator()(const char *p,int len){return f(p,len);}
  BOOM::String &getKey() {return key;}
  const BOOM::String &getKey() const {return key;}
};
}

#endif
