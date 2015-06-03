/***********************************************************************
 GffExon.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_GffExon_H
#define INCL_BOOM_GffExon_H
#include "Map.H"
#include "GffFeature.H"
#include "IndexedFasta.H"
using namespace std;

namespace BOOM {
enum ExonType
  {
    ET_EXON,
    ET_INITIAL_EXON,
    ET_INTERNAL_EXON,
    ET_FINAL_EXON,
    ET_SINGLE_EXON
  };
class GffTranscript;
ostream &operator<<(ostream &,ExonType);

class ExonTypeInitializer
{
private:
  static ExonTypeInitializer e;
  ExonTypeInitializer();
};

class GffExon
{
public:
  GffExon(BOOM::GffFeature &,BOOM::GffTranscript &);
  GffExon(ExonType,int begin,int end,BOOM::GffTranscript &parent,
	  bool hasScore,double score,bool hasFrame,int frame);
  void loadSequence(const BOOM::String &substrate);
  void loadSequence(BOOM::IndexedFasta &substrate);
  BOOM::String &getSequence();
  ExonType getExonType() const;
  void changeExonType(ExonType);
  BOOM::GffTranscript &getParent();
  char getStrand() const;
  const BOOM::String &getSource() const;
  const BOOM::String &getSubstrate() const;
  int getBegin();
  int getEnd();
  int length() { return end-begin; }
  void setBegin(int);
  void setEnd(int);
  void toGff(ostream &);
  float getScore() { return score; }
  bool hasDonor();
  bool hasAcceptor();
private:
  static BOOM::Map<BOOM::String,ExonType> exonTypeNames;
  int frame;  
  int begin, end;            
  double score;
  bool hasScore;  
  bool hasFrame;  
  BOOM::GffTranscript &parent;
  ExonType exonType;
  BOOM::String sequence;
  friend class ExonTypeInitializer;
  static void initExonTypeNames();
};
}

#endif
