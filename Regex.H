/***********************************************************************
 Regex.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Regex_H
#define INCL_BOOM_Regex_H
#include "String.H"
#include "Map.H"
using namespace std;

extern "C" {
#include "regex.h"
}

namespace BOOM {
class Regex {
public:
  Regex(const BOOM::String &regex);
  ~Regex();
  BOOM::String operator[](int whichSubexpression); // first is [1]
  BOOM::String getEntireMatch();
  BOOM::String substitute(const BOOM::String &substrate,
			const BOOM::String &replacement);
  BOOM::Vector<BOOM::String*> *split(const BOOM::String &substrate);
  bool match(const BOOM::String &);
  bool search(const BOOM::String &);
  int getNumSubexpressions() const;
  static BOOM::String substitute(const BOOM::String &regex,
			       const BOOM::String &replacement,
			       const BOOM::String &substrate);
  static BOOM::Vector<BOOM::String*> *split(const BOOM::String &regex,
				    const BOOM::String &substrate);
  static bool match(const BOOM::String &regex,
		    const BOOM::String &substrate);
  static bool search(const BOOM::String &regex,
		     const BOOM::String &substrate);
  void getMatchIndices(int regNum,int &start,int &end);
  operator String() {return regex;}
private:
  re_registers reg;
  re_pattern_buffer patbuf;
  int numOfSubexpress;
  BOOM::String regex;
  const BOOM::String *substrate;
  static Map<String,Regex*> precompiled;
  BOOM::String preprocess(const BOOM::String &);
  void compile();
};
}

#endif
