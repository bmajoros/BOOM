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
#include "Strand.H"
#include "Essex.H"
#include "ProteinTrans.H"
using namespace std;

namespace BOOM {
class GffTranscript {
public:
  GffTranscript(const BOOM::String &transcriptId,
		const BOOM::String &substrate,
		char strand,const BOOM::String &source);
  GffTranscript(const GffTranscript &);
  GffTranscript();
  virtual ~GffTranscript();
  GffTranscript &operator=(const GffTranscript &);
  void loadSequence(const BOOM::String &substrate);//CDS & UTR
  void loadSequence(BOOM::IndexedFasta &);//CDS & UTR
  BOOM::String getSequence(); // CDS only!
  BOOM::String getFullSequence(); // UTR & CDS
  int getSplicedLength() const;//includes CDS and UTR
  int getCDSlength() const;//total length of all CDS
  inline int getNumExons() const { return exons.size(); }// coding only
  inline int numExons() const { return getNumExons(); }// coding only
  BOOM::GffExon &getIthExon(int); // coding only
  bool isCoding() const;
  void deleteIthExon(int); // coding only
  inline int numUTR() const { return UTR.size(); }
  inline GffExon &getIthUTR(int i) { return *UTR[i]; }
  void splitUTRandCDS(const String &genome,int startCodon,
		      const Set<String> &stopCodons); 
  // ^ all exons must either be stored in UTR or exons attribute, but not both
  BOOM::GffFeature *getStartCodon();
  BOOM::GffFeature *getStopCodon();
  char getStrand();
  const BOOM::String &getSource() const;
  const BOOM::String &getSubstrate() const;
  void setSubstrate(const String &);
  const BOOM::String &getTranscriptId() const;
  void setTranscriptId(const String id) { transcriptId=id; }
  const BOOM::String &getGeneId() const;
  void setGeneId(const BOOM::String &);
  double getScore() const;
  int getBegin() const; // includes UTR
  int getEnd() const; // includes UTR
  int getExtent() const; // genomic length (unspliced) = end-begin
  void getCDSbeginEnd(int &begin,int &end); // begin<end
  void addExon(BOOM::GffExon *); // coding only -- no UTR!
  void addUTR(GffExon *);
  void printOn(ostream &) const;
  void setScore(double);
  void setStartCodon(BOOM::GffFeature *);
  void setStopCodon(BOOM::GffFeature *);
  void setStrand(char);
  void sortExons(); // translation order - coding only (no UTR)
  void sortUTR(); // transcription order
  void sort(Vector<GffExon*> &);
  void setExonTypes(); // coding only (no UTR)
  void setUTRtypes();
  void toGff(ostream &);
  void extendFinalExonBy3(); // to include stop codon
  BOOM::Vector<BOOM::GffExon*>::iterator getExons() { return exons.begin();}
  BOOM::Vector<BOOM::GffExon*>::iterator getExonsEnd() {return exons.end();}
  BOOM::Vector<BOOM::GffExon*>::iterator getUTR() { return UTR.begin();}
  BOOM::Vector<BOOM::GffExon*>::iterator getUTRend() {return UTR.end();}
  void getUTR5(Vector<GffExon*> &);
  void getUTR3(Vector<GffExon*> &);
  void getUTR(Vector<GffExon*> &UTR5,Vector<GffExon*> &UTR3);
  int getUTR5length() const;
  void getRawExons(Vector<GffExon*> &shouldDelete);//combines CDS and UTR
  static void deleteExons(Vector<GffExon*> &);
  int mapToTranscriptCoords(int genomicCoord);//UTR & CDS; -1 if not in exon
  int mapToGenomicCoords(int transcriptCoord);//UTR & CDS
  void computePhases();
  String getProtein() { return ProteinTrans::translate(getSequence()); }
  Essex::CompositeNode *toEssex() const;
  bool identical(const GffTranscript &) const; 
  // ^ you MUST call sortExons() & sortUTR() first!!!
  static int genomicToSplicedCoords(int genomicCoord,
			     const Vector<GffExon*> &rawExons);//-1 = not found
  static int splicedToGenomicCoords(int splicedCoord,
			     const Vector<GffExon*> &rawExons);
  const Vector<GffExon*> &peekUTR() const { return UTR; }
  const Vector<GffExon*> &peekExons() const { return exons; }
  bool hasUTR5() const;
  bool hasUTR3() const;
private:
  int begin, end;  // actually coordinates of CDS portion of gene
  double score;
  Strand strand;
  bool hasScore;
  BOOM::Vector<BOOM::GffExon*> exons; // actually CDS portions of exons
  BOOM::Vector<BOOM::GffExon*> UTR;
  BOOM::String transcriptId, geneId;
  BOOM::String substrate;    
  BOOM::String source;       
  BOOM::GffFeature *startCodon, *stopCodon; // ### not used!!
  void splitUTRandCDSfw(const String &genome,GffExon *startExon,
			int startExonIndex,int startCodon,
			const Set<String> &stopCodons); 
  void splitUTRandCDSrev(const String &genome,GffExon *startExon,
			 int startExonIndex,int startCodon,
			 const Set<String> &stopCodons); 
  void appendExons(const Vector<GffExon*> &,Essex::CompositeNode *) const;
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
