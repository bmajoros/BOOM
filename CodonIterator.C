/****************************************************************
 CodonIterator.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "CodonIterator.H"
using namespace std;
using namespace BOOM;

/****************************************************************
                          struct Codon
 ****************************************************************/

bool Codon::valid() const
{
  return codon.length()==3;
}



bool Codon::isStop() const
{
  return codon=="TAG" || codon=="TGA" || codon=="TAA";
}



/****************************************************************
                          class CodonIterator
 ****************************************************************/

CodonIterator::CodonIterator(const GffTranscript &transcript,
			     const String &substrate)
  : transcript(transcript), substrate(substrate),
    exons(transcript.peekExons()), atgExon(0), atgPosInExon(0)
{
  if(transcript.getStrand()!=FORWARD_STRAND) 
    throw "CodonIterator requires forward-strand transcripts"; // ###
  utr5len=transcript.getUTR5length();
  reset();
}



CodonIterator::CodonIterator(const GffTranscript &transcript,
			     const Vector<GffExon*> &rawExons,
			     const String &substrate)
  : transcript(transcript), substrate(substrate), exons(rawExons),
    atgExon(0), atgPosInExon(0)
{
  // This assumes forward-strand!

  // Sum the 5' UTR length
  const Vector<GffExon*> &UTR=transcript.peekUTR();
  utr5len=0;
  for(Vector<GffExon*>::const_iterator cur=UTR.begin(), end=UTR.end() ; 
      cur!=end ; ++cur) {
    const GffExon *utr=*cur;
    if(utr->isUTR5()) utr5len+=utr->length();
  }

  // Find the raw exon containing the start codon
  const int cdsBegin=transcript.getIthExon(0).getBegin();
  const int numRaw=rawExons.size();
  for(int i=0 ; i<numRaw ; ++i) {
    const GffExon *exon=rawExons[i];
    if(exon->contains(cdsBegin)) {
      atgExon=i;
      atgPosInExon=cdsBegin-exon->getBegin();
      break;
    }
  }
  reset();
}



void CodonIterator::reset()
{
  /*
  if(transcript.numExons()<1) throw "Transcript has no exons in CodonIterator";
  currentExonIndex=0;
  currentExon=&transcript.getIthExon(currentExonIndex);
  if(currentExon->length()<1) throw "Zero-length exon found in CodonIterator";
  posWithinExon=0;
  splicedPos=transcript.getUTR5length();
  cdsPos=0;
  */
  if(exons.size()<1) throw "No exons in CodonIterator";
  currentExonIndex=atgExon;
  currentExon=exons[atgExon];
  if(currentExon->length()<1) throw "Zero-length exon found in CodonIterator";
  posWithinExon=atgPosInExon;
  splicedPos=utr5len;
  cdsPos=0;
}



bool CodonIterator::nextCodon(Codon &codon)
{
  // Invariant: if currentExon!=NULL, then character pointed to by
  // posWithinExon is a valid coding nucleotide

  codon.codon="";
  codon.exon=currentExon;
  if(!currentExon) return false;
  int exonBegin=currentExon->getBegin(), exonLen=currentExon->length();
  codon.globalCoord=exonBegin+posWithinExon; 
  codon.splicedCoord=splicedPos;
  codon.cdsCoord=cdsPos;
  while(codon.codon.length()<3) {
    const int substratePos=exonBegin+posWithinExon;
    codon.codon+=substrate[substratePos];
    ++posWithinExon;
    ++splicedPos;
    ++cdsPos;
    if(posWithinExon>=exonLen) {
      ++currentExonIndex;
      if(currentExonIndex>=exons.size()) { currentExon=NULL; break; }
      currentExon=exons[currentExonIndex];
      exonBegin=currentExon->getBegin(); exonLen=currentExon->length();
      posWithinExon=0;
    }
  }
  if(codon.codon.length()==3) return true;
  codon.codon="";
  return false;
  /*
  codon.codon="";
  codon.exon=currentExon;
  if(!currentExon) return false;
  int exonBegin=currentExon->getBegin(), exonLen=currentExon->length();
  codon.globalCoord=exonBegin+posWithinExon; 
  codon.splicedCoord=splicedPos;
  codon.cdsCoord=cdsPos;
  while(codon.codon.length()<3) {
    const int substratePos=exonBegin+posWithinExon;
    codon.codon+=substrate[substratePos];
    ++posWithinExon;
    ++splicedPos;
    ++cdsPos;
    if(posWithinExon>=exonLen) {
      ++currentExonIndex;
      if(currentExonIndex>=transcript.numExons()) { currentExon=NULL; break; }
      currentExon=&transcript.getIthExon(currentExonIndex);
      exonBegin=currentExon->getBegin(), exonLen=currentExon->length();
      posWithinExon=0;
    }
  }
  if(codon.codon.length()==3) return true;
  codon.codon="";
  return false;
   */
}



/*
bool CodonIterator::more() const
{
  if(!currentExon) return false;
  const int L=transcript.getCDSlength();
  const int remaining=L-cdsPos;
  return remaining>=3;
}
*/


