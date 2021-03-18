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
#include "Comparator.H"
using namespace std;
using namespace BOOM;


class GffGene {
public:
  GffGene(const String &id);
  GffGene(const GffGene &);
  //GffGene &operator=(const GffGene &);
  virtual ~GffGene();
  void addTranscript(GffTranscript *);
  int numTranscripts() const;
  GffTranscript &getIthTranscript(int i);
  GffTranscript *longestTranscript();
  int getBegin() const;
  int getEnd() const;
  char getStrand() const;
  String getSubstrate() const;
  const String &getID() const;
protected:
  String id;
  int begin, end;
  Vector<GffTranscript*> transcripts;
};

struct GffGeneComparator : public Comparator<GffGene> 
{
  bool equal(GffGene &a,GffGene &b) 
  { return a.getBegin()==b.getBegin() && a.getEnd()==b.getEnd(); }
  bool greater(GffGene &a,GffGene &b) 
  { return a.getBegin()>b.getBegin(); }
  bool less(GffGene &a,GffGene &b)
  { return a.getBegin()<b.getBegin(); }
};


#endif

