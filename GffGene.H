/****************************************************************
 GffGene.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GffGene_H
#define INCL_GffGene_H
#include <iostream>
#include "GffTranscript.H"
#include "Vector.H"
using namespace std;
using namespace BOOM;


class GffGene {
public:
  GffGene(const String &id);
  virtual ~GffGene();
  void addTranscript(GffTranscript *);
  int numTranscripts() const;
  GffTranscript &getIthTranscript(int i);
  int getBegin() const;
  int getEnd() const;
  char getStrand() const;
  const String &getID() const;
protected:
  String id;
  int begin, end;
  Vector<GffTranscript*> transcripts;
};

#endif

