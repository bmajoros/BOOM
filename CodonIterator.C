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
  : transcript(transcript), substrate(substrate)
{
  reset();
}


void CodonIterator::reset()
{
  if(transcript.numExons()<1) throw "Transcript has no exons in CodonIterator";
  currentExonIndex=0;
  currentExon=&transcript.getIthExon(currentExonIndex);
  if(currentExon->length()<1) throw "Zero-length exon found in CodonIterator";
  posWithinExon=0;
  splicedPos=0;
}



bool CodonIterator::nextCodon(Codon &codon)
{
  // Invariant: if currentExon!=NULL, then character pointed to by
  // posWithinExon is a valid coding nucleotide

  codon.codon=""; 
  if(!currentExon) return false;
  int exonBegin=currentExon->getBegin(), exonLen=currentExon->length();
  codon.globalCoord=exonBegin+posWithinExon; codon.splicedCoord=splicedPos;
  while(codon.codon.length()<3) {
    const int substratePos=exonBegin+posWithinExon;
    codon.codon+=substrate[substratePos];
    ++posWithinExon;
    ++splicedPos;
    if(posWithinExon>=exonLen) {
      ++currentExonIndex;
      if(currentExonIndex>transcript.numExons()) { currentExon=NULL; break; }
      currentExon=&transcript.getIthExon(currentExonIndex);
      exonBegin=currentExon->getBegin(), exonLen=currentExon->length();
      posWithinExon=0;
    }
  }
  if(codon.codon.length()==3) return true;
  codon.codon="";
  return false;
}




