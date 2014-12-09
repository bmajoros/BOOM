/***********************************************************************
 GffExon.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "GffTranscript.H"
#include "GffExon.H"
#include "ProteinTrans.H"
using namespace std;
using namespace BOOM;


BOOM::Map<BOOM::String,ExonType> BOOM::GffExon::exonTypeNames;
ExonTypeInitializer ExonTypeInitializer::e;



BOOM::GffExon::GffExon(BOOM::GffFeature &feature,
		       BOOM::GffTranscript &parent)
  : parent(parent)
{
  exonType=exonTypeNames[feature.getFeatureType()];
  begin=feature.getBegin();
  end=feature.getEnd();
  score=feature.getScore();
  hasScore=feature.isScored();
  frame=feature.getFrame();
  hasFrame=feature.isFramed();
}



BOOM::GffExon::GffExon(ExonType type,int begin,int end,
			 BOOM::GffTranscript &parent,bool hasScore,
			 double score,bool hasFrame,int frame)
  : exonType(type), begin(begin), end(end), parent(parent), 
    hasScore(hasScore), score(score), hasFrame(hasFrame), frame(frame)
{
}



ostream &BOOM::operator<<(ostream &os,ExonType t)
{
  switch(t)
    {
    case ET_EXON:          os<<"exon";            break;
    case ET_INITIAL_EXON:  os<<"initial-exon";    break;
    case ET_INTERNAL_EXON: os<<"internal-exon";   break;
    case ET_FINAL_EXON:    os<<"final-exon";      break;
    case ET_SINGLE_EXON:   os<<"single-exon";     break;
    }
  return os;
}



ExonType BOOM::GffExon::getExonType() const
{
  return exonType;
}



ExonTypeInitializer::ExonTypeInitializer() 
{
  BOOM::GffExon::initExonTypeNames();
}



BOOM::GffTranscript &BOOM::GffExon::getParent()
{
  return parent;
}



char BOOM::GffExon::getStrand() const
{
  return parent.getStrand();
}



const BOOM::String &BOOM::GffExon::getSource() const
{
  return parent.getSource();
}



const BOOM::String &BOOM::GffExon::getSubstrate() const
{
  return parent.getSubstrate();
}



int BOOM::GffExon::getBegin()  
{
  return begin;
}



int BOOM::GffExon::getEnd()  
{
  return end;
}



void BOOM::GffExon::initExonTypeNames()
{
  exonTypeNames["exon"]=ET_EXON;
  exonTypeNames["initial-exon"]=ET_INITIAL_EXON;
  exonTypeNames["final-exon"]=ET_FINAL_EXON;
  exonTypeNames["internal-exon"]=ET_INTERNAL_EXON;
  exonTypeNames["single-exon"]=ET_SINGLE_EXON;
}



void BOOM::GffExon::toGff(ostream &os)
{
  os << parent.getSubstrate() << "\t"
     << parent.getSource() << "\t"
     << exonType << "\t"
     << begin+1 << "\t"
     << end << "\t";
  if(hasScore) os << score; else os << ".";
  os << "\t" << parent.getStrand() << "\t";
  if(hasFrame) os << frame; else os << ".";
  os << "\ttransgrp=" << parent.getTranscriptId() << endl;
}



void BOOM::GffExon::changeExonType(ExonType e)
{
  exonType=e;
}



void BOOM::GffExon::loadSequence(BOOM::IndexedFasta &substrate)
{
  if(begin<0 || end>substrate.getSize())
    throw BOOM::String("BOOM::GffExon::loadSequence() : bad coords: ")+
      begin+" "+end;
  substrate.load(begin,end-begin,sequence);
  sequence.toupper();
  if(getStrand()=='-')
    sequence=BOOM::ProteinTrans::reverseComplement(sequence);
}



void BOOM::GffExon::loadSequence(const BOOM::String &substrate)
{
  sequence=substrate.substring(begin,end-begin);
  sequence.toupper();
  if(getStrand()=='-')
    sequence=BOOM::ProteinTrans::reverseComplement(sequence);
}



BOOM::String &BOOM::GffExon::getSequence()
{
  return sequence;
}


