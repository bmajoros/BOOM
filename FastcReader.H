/****************************************************************
 FastcReader.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_FastcReader_H
#define INCL_FastcReader_H
#include <iostream>
#include "String.H"
#include "Vector.H"
#include "Regex.H"
#include "File.H"
using namespace std;
using namespace BOOM;

/****************************************************************
 FastcReader : reads FASTC files
 FASTAC files are like FASTA files except for continuous data.
 Deflines start with % instead of >.  Values occur one per line
 after the defline.  The first token after the % on the defline is
 the ID.  Key-value pairs can follow this: /key=value /key=value
 ****************************************************************/

typedef Vector< pair<String,String> > StringPairVector;

class FastcReader {
public:
  FastcReader();
  FastcReader(const String &filename);
  void open(const String &filename);
  bool nextSequence(String &defline,String &ID,BOOM::Vector<double> &data);
  bool eof();
  static StringPairVector *parseDefline(const String &defline,String &ID);
protected:
  static Regex deflineRegex, keyValueRegex;
  File file;
  String nextDefline;
};

#endif

