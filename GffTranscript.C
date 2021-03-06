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
#include "CodonIterator.H"
#include "ProteinTrans.H"
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



BOOM::GffTranscript::GffTranscript()
  : begin(-1), end(-1), startCodon(NULL), stopCodon(NULL), hasScore(false)
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
    exons.push_back(new GffExon(**cur,*this));
  for(Vector<GffExon*>::const_iterator cur=other.UTR.begin(), end=
	other.UTR.end() ; cur!=end ; ++cur)
    UTR.push_back(new GffExon(**cur,*this));
}



GffTranscript &GffTranscript::operator=(const GffTranscript &other)
{
  exons.clear();
  UTR.clear();
  begin=other.begin;
  end=other.end;
  score=other.score;
  strand=other.strand;
  hasScore=other.hasScore;
  transcriptId=other.transcriptId;
  geneId=other.geneId;
  substrate=other.substrate;
  source=other.source;
  startCodon=NULL;
  stopCodon=NULL;

  if(other.startCodon) startCodon=new GffFeature(*other.startCodon);
  if(other.stopCodon) stopCodon=new GffFeature(*other.stopCodon);
  for(Vector<GffExon*>::const_iterator cur=other.exons.begin(), end=
	other.exons.end() ; cur!=end ; ++cur)
    exons.push_back(new GffExon(**cur,*this));
  for(Vector<GffExon*>::const_iterator cur=other.UTR.begin(), end=
	other.UTR.end() ; cur!=end ; ++cur)
    UTR.push_back(new GffExon(**cur,*this));
}



BOOM::GffTranscript::~GffTranscript()
{
  // dtor

  for(BOOM::Vector<BOOM::GffExon*>::iterator cur=exons.begin(), end=exons.end()
	; cur!=end ; ++cur) delete *cur;
  for(BOOM::Vector<BOOM::GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) delete *cur;
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



const BOOM::GffExon &BOOM::GffTranscript::getIthExon(int i) const
{
  return *exons[i];
}



BOOM::GffFeature *BOOM::GffTranscript::getStartCodon()
{
  return startCodon;
}



bool GffTranscript::isCoding() const
{
  return exons.size()>0;
}



BOOM::GffFeature *BOOM::GffTranscript::getStopCodon()
{
  return stopCodon;
}



char BOOM::GffTranscript::getStrand() const
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
     << ".\ttranscript_id=" << transcriptId
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



void BOOM::GffTranscript::addUTR(BOOM::GffExon *exon)
{
  UTR.push_back(exon);
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
  if(exons.size()==0) return;
  sort(exons);
  switch(strand) {
  case '+': 
    if(UTR.size()==0) {
      begin=exons[0]->getBegin();
      end=exons[numExons-1]->getEnd();
    }
    break;
  case '-': 
    if(UTR.size()==0) {
      begin=exons[numExons-1]->getBegin();
      end=exons[0]->getEnd();
    }
    break;
  default:  throw "strand is undefined in GffTranscript::sortExons()";
  }
}



void BOOM::GffTranscript::sort(BOOM::Vector<BOOM::GffExon*> &V)
{
  if(V.empty()) return;
  ExonComparator comp;
  BOOM::VectorSorter<BOOM::GffExon*> sorter(V,comp);
  switch(V[0]->getStrand()) {
  case '+': sorter.sortAscendInPlace(); break;
  case '-': sorter.sortDescendInPlace(); break;
  default:  throw "strand is undefined in GffTranscript::sort()";
  }
}



void BOOM::GffTranscript::sortIncreasing(BOOM::Vector<BOOM::GffExon*> &V) const
{
  ExonComparator comp;
  BOOM::VectorSorter<BOOM::GffExon*> sorter(V,comp);
  sorter.sortAscendInPlace();
}



void BOOM::GffTranscript::sortUTR()
{
  int numUTR=UTR.size();
  if(numUTR==0) return;
  sort(UTR);
  switch(strand) {
  case '+': 
    begin=UTR[0]->getBegin();
    end=UTR[numUTR-1]->getEnd();
    break;
  case '-': 
    begin=UTR[numUTR-1]->getBegin();
    end=UTR[0]->getEnd();
    break;
  default:  throw "strand is undefined in GffTranscript::sortUTR()";
  }
}



void BOOM::GffTranscript::toGff(ostream &os)
{
  const int n=exons.size(), u=UTR.size();
  for(int i=0 ; i<u ; ++i) if(UTR[i]->isUTR5()) UTR[i]->toGff(os);
  for(int i=0 ; i<n ; ++i) exons[i]->toGff(os);
  for(int i=0 ; i<u ; ++i) if(UTR[i]->isUTR3()) UTR[i]->toGff(os);

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
  if(numExons==0) return;
  if(numExons==1) {
    exons[0]->changeExonType(ET_SINGLE_EXON);
    return;
  }
  exons[0]->changeExonType(ET_INITIAL_EXON);
  exons[numExons-1]->changeExonType(ET_FINAL_EXON);
  int nMinus1=numExons-1;
  for(int i=1 ; i<nMinus1 ; ++i)
    exons[i]->changeExonType(ET_INTERNAL_EXON);
}



void BOOM::GffTranscript::setUTRtypes()
{
  // First, handle the case of a single UTR (no coding segment)
  int numUTR=UTR.size();
  if(numUTR==0) return;
  if(numUTR==1 && exons.size()==0) {
    UTR[0]->changeExonType(ET_SINGLE_UTR5);
    return;
  }

  // Assign segments to 5' or 3' UTR
  sortUTR();
  int cdsBegin, cdsEnd;
  getCDSbeginEnd(cdsBegin,cdsEnd); // begin<end
  Vector<GffExon*> UTR5, UTR3;
  if(strand==PLUS_STRAND) {
    for(Vector<GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
	cur!=end ; ++cur) {
      GffExon *exon=*cur;
      if(cdsBegin<0 || exon->getBegin()<cdsBegin) UTR5.push_back(exon);
      else UTR3.push_back(exon);
    }
  }
  else {
    for(Vector<GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
	cur!=end ; ++cur) {
      GffExon *exon=*cur;
      if(cdsBegin<0 || exon->getEnd()>cdsEnd) UTR5.push_back(exon);
      else UTR3.push_back(exon);
    }
  }
  // Assign initial/internal/final
  const int numUTR5=UTR5.size(), numUTR3=UTR3.size();
  if(numUTR5==1) UTR5[0]->changeExonType(ET_SINGLE_UTR5);
  else if(numUTR5>0) {
    UTR5[0]->changeExonType(ET_INITIAL_UTR5);
    UTR5[numUTR5-1]->changeExonType(ET_FINAL_UTR5);
    for(int i=1 ; i<numUTR5-1 ; ++i) 
      UTR5[i]->changeExonType(ET_INTERNAL_UTR5);
  }
  if(numUTR3==1) UTR3[0]->changeExonType(ET_SINGLE_UTR3);
  else if(numUTR3>0) {
    UTR3[0]->changeExonType(ET_INITIAL_UTR3);
    UTR3[numUTR3-1]->changeExonType(ET_FINAL_UTR3);
    for(int i=1 ; i<numUTR3-1 ; ++i) 
      UTR3[i]->changeExonType(ET_INTERNAL_UTR3);
  }
}



void GffTranscript::getCDSbeginEnd(int &cdsBegin,int &cdsEnd) const
{
  if(exons.size()==0) { cdsBegin=cdsEnd=-1; return; }
  cdsBegin=exons[0]->getBegin();
  cdsEnd=exons[0]->getEnd();
  for(Vector<GffExon*>::const_iterator cur=exons.begin(), end=exons.end()
	; cur!=end ; ++cur) {
    GffExon *exon=*cur;
    const int exonBegin=exon->getBegin(), exonEnd=exon->getEnd();
    if(exonBegin<cdsBegin) cdsBegin=exonBegin;
    if(exonEnd>cdsEnd) cdsEnd=exonEnd;
  }
}




void BOOM::GffTranscript::loadSequence(BOOM::IndexedFasta &substrate)
{
  int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i) {
      BOOM::GffExon *exon=exons[i];
      exon->loadSequence(substrate);
    }
  int numUTR=UTR.size();
  for(int i=0 ; i<numUTR ; ++i) {
      BOOM::GffExon *exon=UTR[i];
      exon->loadSequence(substrate);
    }
}



void BOOM::GffTranscript::loadSequence(const BOOM::String &substrate)
{
  int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i) {
      BOOM::GffExon *exon=exons[i];
      exon->loadSequence(substrate);
    }
  int numUTR=UTR.size();
  for(int i=0 ; i<numUTR ; ++i) {
      BOOM::GffExon *exon=UTR[i];
      exon->loadSequence(substrate);
    }
}



BOOM::String BOOM::GffTranscript::getSequence() const // CDS only!
{
  BOOM::String sequence;
  int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i) {
    BOOM::GffExon *exon=exons[i];
    sequence+=exon->getSequence();
  }
  return sequence;
}



BOOM::String BOOM::GffTranscript::getFullSequence() // CDS & UTR
{
  setUTRtypes();
  BOOM::String sequence;
  int numExons=exons.size(), numUTR=UTR.size(), u;
  for(u=0 ; u<numUTR ; ++u) {
    BOOM::GffExon *exon=UTR[u];
    if(exon->isUTR5()) sequence+=exon->getSequence();
    else break; }
  for(int i=0 ; i<numExons ; ++i) sequence+=exons[i]->getSequence();
  for( ; u<numUTR ; ++u) sequence+=UTR[u]->getSequence();
  return sequence;
}



const BOOM::String &BOOM::GffTranscript::getGeneId() const
{
  //return geneId.isEmpty() ? transcriptId : geneId;
  return geneId;
}



/*
void GffTranscript::extendCDSby3()
{
  int cdsBegin, cdsEnd;
  getCDSbeginEnd(cdsBegin,cdsEnd);
  
}
*/



/*
  This function can fail if the next raw exon is 1bp or 2bp long (which should
  never happen anyway!).
 */
void GffTranscript::extendFinalExonBy3()
{
  const int lastIndex=exons.size()-1;
  if(lastIndex<0) throw "Empty transcript in extendFinalExonBy3()";
  GffExon &lastExon=*exons[lastIndex];
  Vector<GffExon*> rawExons;
  getRawExons(rawExons);
  if(strand=='+') {
    int exonEnd=lastExon.getEnd();
    int found=
      findExonOverlapping(rawExons,Interval(lastExon.getBegin(),exonEnd));
    GffExon *rawExon=rawExons[found];
    if(exonEnd+3<rawExon->getEnd())
      lastExon.setEnd(exonEnd+3);
    else if(found+1<rawExons.size()) {
      int added=rawExon->getEnd()-exonEnd;
      lastExon.setEnd(rawExon->getEnd());
      int stillNeed=3-added;
      GffExon *nextRaw=rawExons[found+1];
      int begin=nextRaw->getBegin();
      GffExon *exon=new GffExon(ET_EXON,begin,begin+stillNeed,*this,
				false,0,false,0);
      exons.push_back(exon);
      sortExons();
    }
  }
  else throw "GffTranscript::extendFinalExonBy3() not implemented for - strand";
  deleteExons(rawExons);
  trimOverlaps();
}



int GffTranscript::findExonOverlapping(const Vector<GffExon*> exons,
					    Interval interval)
{
  const int numExons=exons.size();
  for(int i=0 ; i<numExons ; ++i) {
    GffExon *exon=exons[i];
    if(exon->getBegin()<interval.getEnd() &&
       interval.getBegin()<exon->getEnd()) return i;
  }
  return -1;
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
  for(Vector<GffExon*>::const_iterator cur=UTR.begin(), end=UTR.end()
	; cur!=end ; ++cur)
    L+=(*cur)->length();
  return L;
}



int BOOM::GffTranscript::getCDSlength() const
{
  int L=0;
  for(Vector<GffExon*>::const_iterator cur=exons.begin(), end=exons.end()
	; cur!=end ; ++cur)
    L+=(*cur)->length();
  return L;
}



int GffTranscript::getExtent() const
{
  return end-begin;
}



void GffTranscript::deleteIthExon(int i)
{
  exons.cut(i);
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



int GffTranscript::getUTR5length() const
{
  int L=0;
  for(Vector<GffExon*>::const_iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=*cur;
    if(exon->isUTR5()) L+=exon->getLength();
  }
  return L;
}



void GffTranscript::getUTR5(Vector<GffExon*> &utr5)
{
  for(Vector<GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=*cur;
    if(exon->isUTR5()) utr5.push_back(exon);
  }
}



void GffTranscript::getUTR3(Vector<GffExon*> &utr3)
{
  for(Vector<GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=*cur;
    if(exon->isUTR3()) utr3.push_back(exon);
  }
}



void GffTranscript::getUTR(Vector<GffExon*> &UTR5,Vector<GffExon*> &UTR3)
{
  for(Vector<GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=*cur;
    Vector<GffExon*> &V=exon->isUTR5() ? UTR5 : UTR3;
    V.push_back(exon);
  }
}



void GffTranscript::forgetCDS()
{
  Vector<GffExon*> raw;
  getRawExons(raw);
  deleteExons(exons); exons.clear();
  deleteExons(UTR); UTR.clear();
  exons=raw;
}



void GffTranscript::getRawExons(Vector<GffExon*> &into) const
{
  into.clear();
  for(Vector<GffExon*>::const_iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=new GffExon(**cur,const_cast<GffTranscript&>(*this));
    exon->changeExonType(ET_EXON);
    exon->setFrame(0);
    into.push_back(exon);
  }
  for(Vector<GffExon*>::const_iterator cur=exons.begin(), end=exons.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=new GffExon(**cur,const_cast<GffTranscript&>(*this));
    exon->changeExonType(ET_EXON);
    exon->setFrame(0);
    into.push_back(exon);
  }
  sortIncreasing(into);
  int N=into.size();
  for(int i=0 ; i+1<N ; ++i) {
    GffExon *exon=into[i], *next=into[i+1];
    if(exon->getEnd()>=next->getBegin()) {
      int overlap=exon->getEnd()-next->getBegin();
      if(overlap>0) {
	String &seq=exon->getSequence(); int L=seq.length();
	if(L>0) seq.padOrTruncate(L-overlap);
      }
      exon->setEnd(next->getEnd());
      exon->getSequence()+=next->getSequence();
      into.cut(i+1);
      --i; --N;
    }
  }
}



void GffTranscript::deleteExons(Vector<GffExon*> &V)
{
  for(Vector<GffExon*>::iterator cur=V.begin(), end=V.end() ; cur!=end ;
      ++cur) delete *cur;
}


int GffTranscript::mapToTranscriptCoords(int genomicCoord)
{
  Vector<GffExon*> rawExons;
  getRawExons(rawExons);
  int sum=0, ret=-1;
  for(Vector<GffExon*>::iterator cur=rawExons.begin(), end=rawExons.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=*cur;
    if(exon->getStrand()!=FORWARD_STRAND) INTERNAL_ERROR;
    const int exonBegin=exon->getBegin(), exonEnd=exon->getEnd(),
      exonLen=exon->getLength();
    if(genomicCoord<exonBegin) break;
    if(genomicCoord<exonEnd) { ret=sum+genomicCoord-exonBegin; break; }
    sum+=exonLen;
  }
  deleteExons(rawExons);
  return ret;
}



int GffTranscript::mapToGenomicCoords(int transcriptCoord)
{
  Vector<GffExon*> rawExons;
  getRawExons(rawExons);
  int ret;
  Vector<GffExon*>::iterator cur=rawExons.begin(), end=rawExons.end();
  for(; cur!=end ; ++cur) {
    GffExon *exon=*cur;
    if(exon->getStrand()!=FORWARD_STRAND) INTERNAL_ERROR;
    const int exonBegin=exon->getBegin(), exonLen=exon->getLength();
    if(transcriptCoord<exonLen) { ret=exonBegin+transcriptCoord; break; }
    transcriptCoord-=exonLen;
  }
  if(cur==end) INTERNAL_ERROR; // coord was past end of transcript
  deleteExons(rawExons);
  return ret;
}



void GffTranscript::computePhases()
{
  sortExons();
  int phase=0;
  for(Vector<GffExon*>::iterator cur=exons.begin(), end=exons.end() ;
      cur!=end ; ++cur) {
    GffExon *exon=*cur;
    exon->setFrame(phase);
    phase=(phase+exon->length())%3;
  }
}



void GffTranscript::setSubstrate(const String &s)
{
  substrate=s;
}



void GffTranscript::splitUTRandCDS(const String &genome,int startCodon,
				   const Set<String> &stopCodons)
{
  // The startCodon coordinate must be the leftmost base of the codon,
  // regardless of strand.

  // Put everything into the exons attribute, make sure UTR is empty
  if(UTR.size()>0) {
    if(exons.size()>0) 
      throw "GffTranscript::splitUTRandCDS(): both UTR and CDS already exist";
    exons=UTR;
    UTR.clear();
  }
  else if(exons.size()==0)
    throw "GffTranscript::splitUTRandCDS(): no exons found";

  // Find the exon containing the start codon
  int numExons=exons.size();
  GffExon *startExon=NULL;
  int startExonIndex=-1;
  for(int i=0 ; i<numExons ; ++i)
    if(exons[i]->contains(startCodon)) 
      { startExon=exons[i]; startExonIndex=i; break; }
  if(!startExon) { UTR=exons; exons.clear(); return; }
  if(strand==FORWARD_STRAND) 
    splitUTRandCDSfw(genome,startExon,startExonIndex,startCodon,stopCodons);
  else 
    splitUTRandCDSrev(genome,startExon,startExonIndex,startCodon,stopCodons);
}



void GffTranscript::splitUTRandCDSfw(const String &genome,GffExon *startExon,
				     int startExonIndex,int startCodon,
				     const Set<String> &stopCodons)
{
  // Move all 5' exons to UTR
  for(int i=0 ; i<startExonIndex ; ++i) {
    GffExon *exon=exons[i];
    exon->changeExonType(ET_UTR5);
    UTR.push_back(exon);
  }
  exons.cut(0,startExonIndex);
  if(startCodon>startExon->getBegin()) {
    GffExon *exon=new GffExon(ET_UTR5,startExon->getBegin(),startCodon,
			      *this,false,0.0,false,0);
    UTR.push_back(exon);
    startExon->setBegin(startCodon);
  }
  startExon->changeExonType(ET_INITIAL_EXON);

  // Find the stop codon
  CodonIterator iter(*this,genome);
  Codon codon;
  GffExon *stopExon=NULL;
  while(iter.nextCodon(codon))
    if(stopCodons.isMember(codon.codon))
      {stopExon=codon.exon; break;}

  // Move all 3' exons to UTR
  if(stopExon) {
    GffExon *exon=new GffExon(ET_UTR3,codon.globalCoord+3,stopExon->getEnd(),
			      *this,false,0.0,false,0);
    UTR.push_back(exon);
    stopExon->setEnd(codon.globalCoord+3);
    int utr=-1;
    int numExons=exons.size();
    for(int i=0 ; i<numExons ; ++i) if(exons[i]==stopExon) { utr=i+1; break; }
    if(utr<0) INTERNAL_ERROR;
    for(int i=utr ; i<numExons ; ++i) {
      GffExon *exon=exons[i];
      exon->changeExonType(ET_UTR3);
      UTR.push_back(exon);
    }
    if(numExons>utr) exons.cut(utr,numExons-utr);
  }

  // Set detailed exon types
  setExonTypes();
  setUTRtypes();
}



void GffTranscript::splitUTRandCDSrev(const String &genome,GffExon *startExon,
				      int startExonIndex,int startCodon,
				      const Set<String> &stopCodons)
{
  throw "GffTranscript::splitUTRandCDSrev() not implemented";
}



Essex::CompositeNode *GffTranscript::toEssex(bool reverseStrand,
					     int substrateLen) const
{
  Essex::CompositeNode *root=new Essex::CompositeNode("transcript");
  root->append("ID",transcriptId);
  root->append("gene",geneId);
  root->append("type",isCoding() ? "protein-coding" : "noncoding");
  root->append("substrate",substrate);
  root->append("source",source);
  int b=reverseStrand ? substrateLen-begin : begin;
  int e=reverseStrand ? substrateLen-end : end;
  if(b>e) { int t=b; b=e; e=t; }
  root->append("begin",b);
  root->append("end",e);
  if(hasScore) root->append("score",float(score));
  else root->append("score",".");
  Strand s=reverseStrand ? complement(strand) : strand;
  root->append("strand",s==FORWARD_STRAND ? "+" : "-");
  if(exons.size()>0) {
    Essex::CompositeNode *exonsNode=new Essex::CompositeNode("exons");
    appendExons(exons,exonsNode,true,reverseStrand,substrateLen);
    root->append(exonsNode);
  }
  if(UTR.size()>0) {
    Essex::CompositeNode *utrNode=new Essex::CompositeNode("UTR");
    appendExons(UTR,utrNode,false,reverseStrand,substrateLen);
    root->append(utrNode);
  }
  if(isCoding()) 
    root->append("translation",ProteinTrans::translate(getSequence()));
  return root;
}



void GffTranscript::appendExons(const Vector<GffExon*> &exons,
				Essex::CompositeNode *root,
				bool hasPhase,bool revComp,int L) const
{
  for(Vector<GffExon*>::const_iterator cur=exons.begin(), end=exons.end() ;
      cur!=end ; ++cur) {
    const GffExon *exon=*cur;
    const char *type=toString(exon->getExonType());
    Essex::CompositeNode *node=new Essex::CompositeNode(type);
    root->append(node);
    int b=revComp ? L-exon->getBegin() : exon->getBegin();
    int e=revComp ? L-exon->getEnd() : exon->getEnd();
    if(b>e) { int t=b; b=e; e=t; }
    node->append(b);
    node->append(e);
    node->append(exon->getScore());
    Strand strand=exon->getStrand();
    Strand s=revComp ? complement(strand) : strand;
    node->append(s==FORWARD_STRAND ? "+" : "-");
    if(hasPhase) node->append(exon->getFrame());
    else node->append(".");
  }
}



bool GffTranscript::identical(const GffTranscript &other) const
{
  if(begin!=other.begin ||
     end!=other.end ||
     strand!=other.strand ||
     substrate!=other.substrate) return false;
  const int numExons=exons.size();
  if(numExons!=other.exons.size()) return false;
  const int numUTR=UTR.size();
  if(numUTR!=other.UTR.size()) return false;
  for(int i=0 ; i<numExons ; ++i) 
    if(!exons[i]->identical(*other.exons[i])) return false;
  for(int i=0 ; i<numUTR ; ++i) 
    if(!UTR[i]->identical(*other.UTR[i])) return false;
  return true;
}



int GffTranscript::genomicToSplicedCoords(int genomicCoord,
			   const Vector<GffExon*> &rawExons)
{
  if(rawExons.size()>0 && rawExons[0]->getStrand()!=FORWARD_STRAND) 
    throw "GffTranscript::genomicToSplicedCoords() requires forward-strand features";
  int leftSum=0;
  for(Vector<GffExon*>::const_iterator cur=rawExons.begin(), end=
	rawExons.end() ; cur!=end ; ++cur) {
    GffExon *exon=*cur;
    if(exon->getBegin()>genomicCoord) // it was in the preceding intron
      return -1;
    if(exon->contains(genomicCoord))
      return leftSum+genomicCoord-exon->getBegin();
    else leftSum+=exon->length();
  }
  return -1;
}



int GffTranscript::splicedToGenomicCoords(int splicedCoord,
			   const Vector<GffExon*> &rawExons)
{
  if(rawExons.size()>0 && rawExons[0]->getStrand()!=FORWARD_STRAND) 
    throw "GffTranscript::splicedToGenomicCoords() requires forward-strand features";
  int splicedExonBegin=0;
  for(Vector<GffExon*>::const_iterator cur=rawExons.begin(), end=
	rawExons.end() ; cur!=end ; ++cur) {
    GffExon *exon=*cur;
    const int exonLength=exon->length();
    const int splicedExonEnd=splicedExonBegin+exonLength;
    if(splicedExonEnd>splicedCoord)
      return exon->getBegin()+splicedCoord-splicedExonBegin;
    else splicedExonBegin+=exonLength;
  }
  INTERNAL_ERROR;
}



bool GffTranscript::hasUTR5() const
{
  for(Vector<GffExon*>::const_iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur)
    if((*cur)->isUTR5()) return true;
  return false;
}



bool GffTranscript::hasUTR3() const
{
  for(Vector<GffExon*>::const_iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur)
    if((*cur)->isUTR3()) return true;
  return false;
}



void GffTranscript::getIntrons(Vector<Interval> &into) const
{
  Vector<GffExon*> rawExons;
  getRawExons(rawExons);
  sortIncreasing(rawExons);
  const int n=rawExons.size();
  for(int i=0 ; i<n-1 ; ++i) {
    GffExon *exon=rawExons[i], *next=rawExons[i+1];
    //if(strand==FORWARD_STRAND)
      into.push_back(Interval(exon->getEnd(),next->getBegin()));
      //else
      //into.push_back(Interval(next->getEnd(),exon->getBegin()));
  }
  deleteExons(rawExons);
}



void GffTranscript::reverseComplement(const int L)
{
  /*
  BOOM::Vector<BOOM::GffExon*> exons; // actually CDS portions of exons
  BOOM::Vector<BOOM::GffExon*> UTR;
   */

  begin=L-begin; //-1;
  end=L-end; //-1;
  int temp=begin; begin=end; end=temp;
  strand=complement(strand);
  for(Vector<GffExon*>::iterator cur=exons.begin(), end=exons.end() ;
      cur!=end ; ++cur) 
    (*cur)->reverseComplement(L);
  for(Vector<GffExon*>::iterator cur=UTR.begin(), end=UTR.end() ;
      cur!=end ; ++cur) 
    (*cur)->reverseComplement(L);
}



int GffTranscript::stopCodonGlobalCoord() const
{
  int begin, end;
  getCDSbeginEnd(begin,end);
  return strand==FORWARD_STRAND ? end-3 : begin;
}



void GffTranscript::trimOverlaps()
{
  int numUTR=UTR.size();
  for(int i=0 ; i<numUTR ; ++i) {
    GffExon &utr=*UTR[i]; int utrBegin=utr.getBegin(), utrEnd=utr.getEnd();
    for(Vector<GffExon*>::iterator cur=exons.begin(), end=exons.end() ;
	cur!=end ; ++cur) {
      GffExon &exon=**cur; int exonBegin=exon.getBegin(), exonEnd=exon.getEnd();
      if(utrBegin>=exonBegin && utrEnd<=exonEnd) {
	UTR.cut(i); delete &utr; --numUTR; --i; break; }
      if(utrBegin<exonBegin && utrEnd>exonBegin) {
	int overlap=utrEnd-exonBegin;
	utr.setEnd(exonBegin);
	String &S=utr.getSequence();
	if(!S.empty()) S.padOrTruncate(S.length()-overlap);
      }
      else if(utrBegin<exonEnd && utrEnd>exonEnd) {
	int overlap=exonEnd-utrBegin;
	utr.setBegin(exonEnd);
	String &S=utr.getSequence();
	if(!S.empty()) S=S.substring(overlap);
      }
    }
  }
}




