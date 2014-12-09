/***********************************************************************
 FastaWriter.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_FastaWriter_H
#define INCL_BOOM_FastaWriter_H
#include "String.H"
#include <iostream>
#include "Regex.H"
using namespace std;

namespace BOOM {
class FastaWriter
{
public:
  FastaWriter(int width=60);
  void writeFasta(const BOOM::String &defline,const BOOM::String &sequence,
		  const BOOM::String &filename);
  void writeFastaFromCharPtr(const BOOM::String &defline,
			     const char *sequence,
			 const BOOM::String &filename);
  void addToFasta(const BOOM::String &defline,const char *sequence,
		  ostream &);
  void addToFasta(const BOOM::String &defline,const BOOM::String &sequence,
		  ostream &);
private:
  int width;
  BOOM::Regex newline, greater;
};
}


#endif
