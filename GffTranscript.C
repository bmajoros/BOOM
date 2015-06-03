/***********************************************************************
 GffTranscript.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "VectorSorter.H"
#include "GffTranscript.H"
using namespace std;



BOOM::GffTranscript::GffTranscript(const BOOM::String &transcriptId,
			     const BOOM::String &substrate,
			     char strand,const BOOM::String &source)
  : transcriptId(transcriptId), strand(strand), begin(-1), end(-1),
    substrate(substrate), source(source), startCodon(NULL),
    stopCodon(NULL), hasScore(false)
{ 
  // ctor
}



GffTranscript::GffTranscript(const GffTranscript &other)
  : begin(other.begin), end(other.end), score(other.score),
    strand(other.strand), hasScore(other.hasScore), 
    transcriptId(other.transcriptId), geneId(other.geneId),
    substrate(other.substrate), source(other.source),
    startCodon(NULL), stopCodon(NULL)
{
  if(other.startCodon) startCodon=new GffFeature(*other.startCodon);
  if(other.stopCodon) stopCodon=new GffFeature(*other.stopCodon);
  for(Vector<GffExon*>::const_iterator cur=other.exons.begin(), end=
	other.exons.end() ; cur!=end ; ++cur)
    exons.push_back(new GffExon(**cur));
}



BOOM::GffTranscript::~GffTranscript()
{
  // dtor

  BOOM::Vector<BOOM::GffExon*>::iterator cur=exons.begin(), end=exons.end();
  for(; cur!=end ; ++cur) delete *cur;
  delete startCodon;
  delete stopCodon;
}



ostream &operator<<(ostream &os,const BOOM::GffTranscript &transcript)
{
  transcript.printOn(os);
  return os;
}



BOOM::GffExon &BOOM::GffTranscript::getIthExon(int i)
{
  return *exons[i];
}



BOOM::GffFeature *BOOM::GffTranscript::getStartCodon()
{
  return startCodon;
}



BOOM::GffFeature *BOOM::GffTranscript::getStopCodon()
{
  return stopCodon;
}



char BOOM::GffTranscript::getStrand()
{
  return strand;
}



const BOOM::String &BOOM::GffTranscript::getSource() const
{
  return source;
}



const BOOM::String &BOOM::GffTranscript::getSubstrate() const
{
  return substrate;
}



const BOOM::String &BOOM::GffTranscript::getTranscriptId() const
{
  return transcriptId;
}



double BOOM::GffTranscript::getScore() const
{
  return score;
}



int BOOM::GffTranscript::getBegin() const
{
  return begin;
}



int BOOM::GffTranscript::getEnd() const
{
  return end;
}



void BOOM::GffTranscript:: printOn(ostream &os) const
{
  os << substrate << "\t"
     << source << "\t"
     << "transcript\t"
     << begin+1 << "\t"
     << end << "\t"
     << ".\t"
     << strand << "\t"
     << ".\ttransscript_id=" << transcriptId
     << "; gene_id=" << geneId
     << "; numExons=" << getNumExons();
}



void BOOM::GffTranscript::addExon(BOOM::GffExon *exon)
{
  exons.push_back(exon);
  int exonBegin=exon->getBegin(), exonEnd=exon->getEnd();
  if(begin<0 || exonBegin<begin) begin=exonBegin;
  if(end<0 || exonEnd>end) end=exonEnd;
}



void BOOM::GffTranscript::setScore(double s)
{
  score=s;
  hasScore=true;
}



void BOOM::GffTranscript::setStartCodon(BOOM::GffFeature *s)
{
  startCodon=s;
}



void BOOM::GffTranscript::setStopCodon(BOOM::GffFeature *s)
{
  stopCodon=s;
}



void BOOM::GffTranscript::setStrand(char s)
{
  strand=s;
}



class ExonComparator : public BOOM::Comparator<BOOM::GffExon*>
{
public:
  bool less(BOOM::GffExon *&a,BOOM::GffExon *&b) 
  {return a->getBegin()<b->getBegin();}
  bool greater(BOOM::GffExon *&a,BOOM::GffExon *&b) 
  {return a->getBegin()>b->getBegin();}
  bool equal(BOOM::GffExon *&a,BOOM::GffExon *&b) 
  {return a->getBegin()==b->getBegin();}
};



void BOOM::GffTranscript::sortExons()
{
  int numExons=exons.size();
  ExonComparator comp;
  BOOM::VectorSorter<BOOM::GffExon*> sorter(exons,comp);
  switch(strand)
    {
    case '+': 
      sorter.sortAscendInPlace(); 
      begin=exons[0]->getBegin();
      end=exons[numExons-1]->getEnd();
      break;
    case '-': 
      sorter.sortDescendInPlace(); 
      begin=exons[numExons-1]->getBegin();
      end=exons[0]->getEnd();
      break;
      //default:  throw "strand is undefined in BOOM::GffTranscript::sortExons";
    }
}



void BOOM::GffTranscript::toGff(ostream &os)
{
  int n=exons.size();
  for(int i=0 ; i<n ; ++i)
    {
      BOOM::GffExon *exon=exons[i];
      exon->toGff(os);
    }

  if(startCodon) os << startCodon->toGff();
  if(stopCodon) os << stopCodon->toGff();

  os << substrate << "\t"
     << source << "\t"
     << "transcript\t"
     << begin+1 << "\t"
     << end << "\t";
  if(hasScore)
    os << score << "\t";
  else
    os << ".\t";
  os << strand << "\t"
     << ".\ttranscript_id=" << transcriptId 
     << "; gene_id=" << geneId
     << endl;
}



void BOOM::GffTranscript::setExonTypes()
{
  int numExons=exons.size();
  if(numExons==1)
    {
      exons[0]->changeExonType(ET_SINGLE_EXON);
      return;
    }

  exons[0]->changeExonType(ET_INITIAL_EXON);
  exons[numExons-1]->changeExonType(ET_FINAL_EXON);
  int nMinus1=numExons-1;
  for(int i=1 ; i<nMinus1 ; ++i)
    exons[i]->changeExonType(ET_INTERNAL_EXON);
}



void BOOM::GffTranscript::loadSequence(BOOM::IndexedFasta &substrate)
{
  int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i)
    {
      BOOM::GffExon *exon=exons[i];
      exon->loadSequence(substrate);
    }
}



void BOOM::GffTranscript::loadSequence(const BOOM::String &substrate)
{
  int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i)
    {
      BOOM::GffExon *exon=exons[i];
      exon->loadSequence(substrate);
    }
}



BOOM::String BOOM::GffTranscript::getSequence()
{
  BOOM::String sequence;
  int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i)
    {
      BOOM::GffExon *exon=exons[i];
      sequence+=exon->getSequence();
    }
  return sequence;
}



const BOOM::String &BOOM::GffTranscript::getGeneId() const
{
  return geneId;
}



void GffTranscript::extendFinalExonBy3()
{
  const int lastIndex=exons.size()-1;
  if(lastIndex<0) throw "Empty transcript in extendFinalExonBy3()";
  GffExon &lastExon=*exons[lastIndex];
  if(strand=='+') lastExon.setEnd(lastExon.getEnd()+3);
  else lastExon.setBegin(lastExon.getBegin()-3);
}



void BOOM::GffTranscript::setGeneId(const BOOM::String &id)
{
  geneId=id;
}



int BOOM::GffTranscript::getSplicedLength() const
{
  int L=0;
  for(Vector<GffExon*>::const_iterator cur=exons.begin(), end=exons.end()
	; cur!=end ; ++cur)
    L+=(*cur)->length();
  return L;
}



bool TranscriptComparator::less(BOOM::GffTranscript *&a,
				BOOM::GffTranscript *&b) 
{
  return a->getBegin()<b->getBegin();
}



bool TranscriptComparator::greater(BOOM::GffTranscript *&a,
				   BOOM::GffTranscript *&b) 
{
  return a->getBegin()>b->getBegin();
}



bool TranscriptComparator::equal(BOOM::GffTranscript *&a,
				 BOOM::GffTranscript *&b) 
{
  return a->getBegin()==b->getBegin();
}




