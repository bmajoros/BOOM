/****************************************************************
 CFasta.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_CFasta_H
#define INCL_CFasta_H
#include <iostream>
#include "String.H"
#include "Vector.H"
#include "File.H"
#include "Regex.H"
using namespace std;
using namespace BOOM;

/****************************************************************
 CFASTA : Continuous Fasta format
   >gene312
   0.04  0.68  0.92  0.12
   0.98  0.12  0.84  0.123  0.73
   >gene782
   0.12  0.321 ...etc...
 ****************************************************************/

class CFasta {
public:
  CFasta(const String &filename);
  virtual ~CFasta();
  void close();
  bool getNextSequence(String &defline,String &id,Vector<double> &values);
  bool eof();
protected:
  Regex deflineRegex;
  File f;
  String nextDefline;
  String filename;
};

#endif

