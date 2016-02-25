/****************************************************************
 CodonIterator.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_CodonIterator_H
#define INCL_CodonIterator_H
#include <iostream>
#include "String.H"
#include "GffTranscript.H"
using namespace std;
using namespace BOOM;



/****************************************************************
                          struct Codon
 ****************************************************************/
struct Codon {
  String codon;
  int globalCoord;  // coordinate on substrate
  int splicedCoord; // coordinate in spliced transcript (including 5'UTR)
  int cdsCoord;     // coordinate within spliced CDS (not including UTR)
  GffExon *exon;    // which exon it starts in (it might span an intron)
  bool valid() const;
  bool isStop() const; // human-specific: TAG/TGA/TAA
  Codon() : exon(NULL) {}
};



/****************************************************************
                          class CodonIterator
 ****************************************************************/
class CodonIterator {
public:
  // NOTE: this class currently only works for the forward strand!
  CodonIterator(const GffTranscript &,const String &substrate);
  CodonIterator(const GffTranscript &,const Vector<GffExon*> &rawExons,
		const String &substrate);
  bool nextCodon(Codon &); // return false if no more
protected:
  const GffTranscript &transcript;
  const String &substrate;
  const GffExon *currentExon;
  const Vector<GffExon*> &exons;
  int atgExon, atgPosInExon, utr5len;
  int currentExonIndex;
  int posWithinExon;
  int splicedPos; // includes 5'UTR
  int cdsPos; // position within CDS (spliced), not including UTR
  void reset();
};

#endif

