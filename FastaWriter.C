/***********************************************************************
 FastaWriter.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "FastaWriter.H"
#include <fstream>
#include <string.h>
using namespace std;


BOOM::FastaWriter::FastaWriter(int width)
  : width(width), newline("\n$"), greater("^\\s*>")
{
}



void BOOM::FastaWriter::writeFasta(const BOOM::String &defline,
				 const BOOM::String &sequence,
				 const BOOM::String &filename)
{
  ofstream os(filename.c_str());
  addToFasta(defline,sequence.c_str(),os);
}



void BOOM::FastaWriter::writeFastaFromCharPtr(const BOOM::String &defline,
					    const char *sequence,
					    const BOOM::String &filename)
{
  ofstream os(filename.c_str());
  addToFasta(defline,sequence,os);
}



void BOOM::FastaWriter::addToFasta(const BOOM::String &defline,
				 const BOOM::String &sequence,
				 ostream &os)
{
  addToFasta(defline,sequence.c_str(),os);
}



void BOOM::FastaWriter::addToFasta(const BOOM::String &def,
				 const char *sequence,
				 ostream &os)
{
  BOOM::String defline=def;
  if(newline.search(defline)) defline.chop();
  if(!greater.search(defline)) defline=BOOM::String(">")+defline;

  os << defline << endl;
  int length=strlen(sequence);
  int numLines=int(length/width);
  if(length%width) ++numLines;
  int start=0;

  char *line=new char[width+1];
  line[width]='\0';
  for(int i=0 ; i<numLines ; ++i)
    {
      strncpy(line,sequence+start,width);
      os << line << endl;
      start+=width;
    }
  delete [] line;
  if(length==0) os << endl;
}


