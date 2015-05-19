/***********************************************************************
 GffTranscript.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_GffTranscript_H
#define INCL_BOOM_GffTranscript_H
#include <iostream>
#include "Vector.H"
#include "String.H"
#include "GffExon.H"
#include "IndexedFasta.H"
using namespace std;

namespace BOOM {
class GffTranscript
{
public:
  GffTranscript(const BOOM::String &transcriptId,
		const BOOM::String &substrate,
		char strand,const BOOM::String &source);
  GffTranscript(const GffTranscript &);
  virtual ~GffTranscript();
  void loadSequence(const BOOM::String &substrate);
  void loadSequence(BOOM::IndexedFasta &);
  BOOM::String getSequence();
  int getNumExons() const;
  BOOM::GffExon &getIthExon(int);
  BOOM::GffFeature *getStartCodon();
  BOOM::GffFeature *getStopCodon();
  char getStrand();
  const BOOM::String &getSource() const;
  const BOOM::String &getSubstrate() const;
  const BOOM::String &getTranscriptId() const;
  const BOOM::String &getGeneId() const;
  void setGeneId(const BOOM::String &);
  double getScore() const;
  int getBegin() const;
  int getEnd() const;
  void addExon(BOOM::GffExon *);
  void printOn(ostream &) const;
  void setScore(double);
  void setStartCodon(BOOM::GffFeature *);
  void setStopCodon(BOOM::GffFeature *);
  void setStrand(char);
  void sortExons();
  void setExonTypes();
  void toGff(ostream &);
  BOOM::Vector<BOOM::GffExon*>::iterator getExons() {return exons.begin();}
  BOOM::Vector<BOOM::GffExon*>::iterator getExonsEnd() {return exons.end();}
private:
  int begin, end;  
  double score;
  char strand;
  bool hasScore;
  BOOM::Vector<BOOM::GffExon*> exons;
  BOOM::String transcriptId, geneId;
  BOOM::String substrate;    
  BOOM::String source;       
  BOOM::GffFeature *startCodon, *stopCodon;
};


class TranscriptComparator : public BOOM::Comparator<BOOM::GffTranscript*>
{
public:
  bool less(BOOM::GffTranscript *&,BOOM::GffTranscript *&);
  bool greater(BOOM::GffTranscript *&,BOOM::GffTranscript *&); 
  bool equal(BOOM::GffTranscript *&,BOOM::GffTranscript *&);
};
}

ostream &operator<<(ostream &,const BOOM::GffTranscript &);

#endif
