/***********************************************************************
 MultSeqAlignment.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_MultSeqAlignment_H
#define INCL_MultSeqAlignment_H
#include "String.H"
#include "Vector.H"
#include "Map.H"
#include "AlignmentSeq.H"
#include "Regex.H"
#include "MultiAlignment.H"
#include "Alphabet.H"
#include "BitSet.H"

namespace BOOM {
class MultSeqAlignment
{
public:
  MultSeqAlignment(const MultiAlignment &,const Alphabet &,Symbol gapSymbol);
  MultSeqAlignment(const MultiAlignment &,const Alphabet &,
		   const BitSet &gapSymbols);
  MultSeqAlignment(const Sequence &,const String &seqName,const Alphabet &,
		   Symbol gapSymbol);
  MultSeqAlignment(const Alphabet &,Symbol gapSymbol);
  void addGapSymbol(Symbol);
  virtual ~MultSeqAlignment();
  AlignmentSeq &findOrCreateTrack(const BOOM::String &trackName);
  void addTrack(AlignmentSeq *);
  AlignmentSeq &getIthTrack(int i);
  void releaseTrack(int i); // won't delete it; set it to NULL
  AlignmentSeq &getTrackByName(const BOOM::String &);
  bool trackExists(const String &);
  int getNumTracks() const;
  int getLength() const;
  void deleteSequences();
  void save(const BOOM::String &filename,bool alignLeft=false,
	    bool wantAnno=false);
  void save(ostream &,bool alignLeft=false,bool wantAnno=false);
  void printSlice(ostream &os,int begin,int length,char strand,
		  int lineLength,bool alignLeft=false,bool wantAnno=false) 
    const;
  MultSeqAlignment *getSlice(int begin,int length);
  bool containsGap();
  static int rightmostGapPos(const Sequence &m,const BitSet &gapSymbols);
  const BitSet &getGapSymbols() const;
  Symbol getGapSymbol() const {return gapSymbol;}
  void deleteTargetGaps(int trackID);
  int getPhase() const {return phase;}
  void setPhase(int p);
  void append(MultSeqAlignment &);
  void append(MultSeqAlignment &,int begin,int length);
  void extendToLength(int length);
  double getScore() const;
  void setScore(double);
  Array1D<char> &getAnnotationTrack();
  void enableAnnotation(bool=true);
  void sortTracksByName();
  void capByAnno();
protected:
  MultSeqAlignment(const Alphabet &,const BitSet &gapSymbols);
  BOOM::Vector<AlignmentSeq*> tracks;
  BOOM::Map<BOOM::String,int> nameToTrack;
  bool wantAnnoCaps;
  Array1D<char> annotationTrack;
  bool annotationEnabled;
  int phase;
  const Alphabet &alphabet;
  BitSet gapSymbols;
  Symbol gapSymbol;
  double score;
  
  void convert(const MultiAlignment &);
  void emitAnnotationSlice(ostream &os,int begin,int L);
};
}

#endif
