/***********************************************************************
 MultiAlignment.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_MultiAlignment_H
#define INCL_MultiAlignment_H
#include "String.H"
#include "Vector.H"
#include "Map.H"
#include "AlignmentTrack.H"
#include "Regex.H"

namespace BOOM {
class MultiAlignment
{
public:
  MultiAlignment();
  virtual ~MultiAlignment();
  void loadMAF(const BOOM::String &filename); // only loads the first align
  int loadMAF(istream &,int phase=0); // loads the next alignment
  static void loadMAF(const BOOM::String &filename,
		      BOOM::Vector<MultiAlignment*> &into);
  static MultiAlignment *nextAlignmentFromMAF(istream &);
  static MultiAlignment *combine(BOOM::Vector<MultiAlignment*> &,
				 bool shouldDelete=false);
  void append(const MultiAlignment &);
  void toupper();
  AlignmentTrack &findOrCreateTrack(const BOOM::String &trackName);
  void addTrack(AlignmentTrack *);
  AlignmentTrack &getIthTrack(int i);
  const AlignmentTrack &getIthTrack(int i) const;
  void releaseTrack(int i); // won't delete it; set it to NULL
  AlignmentTrack &getTrackByName(const BOOM::String &);
  int getNumTracks() const;
  int getLength() const;
  void printSlice(ostream &,int begin,int length,char strand,
		  int lineLength=60,String extraTags="") const;
  void printOn(ostream &) const;
  void deleteSequences();
  int getPhase() const {return phase;}
  bool trackExists(const String &name);
  AlignmentTrack *findTrack(BOOM::Regex &);
  const String &getHeader() const {return header;}
  void setHeader(const String &h) {header=h;}
  void deleteTargetGaps(int trackID,char gapSymbol);
  void ensureAllSameLength();
private:
  static BOOM::Regex lineParser, lineParser_empty, alineParser;
  static BOOM::Map<istream*,int> nextPhases;
  static Map<istream*,String> nextHeaders;
  BOOM::Vector<AlignmentTrack*> tracks;
  BOOM::Map<BOOM::String,int> nameToTrack;
  int length, phase;
  String header;
};

ostream &operator<<(ostream &,const MultiAlignment &);    
}

#endif
