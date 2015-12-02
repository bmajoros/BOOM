/***********************************************************************
 FastaReader.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_FastaReader_H
#define INCL_BOOM_FastaReader_H
#include "Vector.H"
#include "String.H"
#include "File.H"
#include "DnaAlphabet.H"
#include "AminoAlphabet.H"
#include "Regex.H"
using namespace std;

/*
  Reads a FASTA file into memory, converts sequence to uppercase,
  and masks strange characters (anything not in the alphabet) using N's.
 */
namespace BOOM {
class FastaReader
{
public:
  FastaReader(const BOOM::String &filename,const Alphabet & 
	      =DnaAlphabet::global());
  static load(const String &filename,String &defline,String &sequence,
	      const Alphabet & =DnaAlphabet::global());
  BOOM::Vector< pair<BOOM::String,BOOM::String> > *readAll();
  bool nextSequence(BOOM::String &defline,BOOM::String &sequence);
  static void parseDefline(const BOOM::String &defline,BOOM::String &id,
			   BOOM::String &remainder);
  static void parseAttributes(const String &remainder,Map<String,String> &attr);
  static BOOM::String getId(const BOOM::String &defline);
  void close() {file.close();}
  bool eof() {return file.eof();}
private:
  BOOM::String cache;
  BOOM::File file;
  const Alphabet &alphabet;
  static BOOM::Regex deflineRegex;
  static BOOM::Regex attrRegex;

  void maskStrangeChars(BOOM::String &);
};
}

#endif
