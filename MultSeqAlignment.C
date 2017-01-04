/***********************************************************************
 MultSeqAlignment.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "MultSeqAlignment.H"
#include "ProteinTrans.H"
#include "DnaAlphabet.H"
#include "VectorSorter.H"
#include "Exceptions.H"
#include <iostream>
#include <fstream>
using namespace std;
using namespace BOOM;



MultSeqAlignment::MultSeqAlignment(const Alphabet &alphabet,Symbol gapSymbol)
  : alphabet(alphabet), gapSymbol(gapSymbol), gapSymbols(alphabet.size()),
    score(1.0), annotationEnabled(false), wantAnnoCaps(false)
{
  gapSymbols.addMember(gapSymbol);
}



MultSeqAlignment::MultSeqAlignment(const Sequence &S,
				   const String &seqName,
				   const Alphabet &alphabet,
				   Symbol gapSymbol)
  : alphabet(alphabet), gapSymbol(gapSymbol), gapSymbols(alphabet.size()),
    score(1.0), annotationEnabled(false), wantAnnoCaps(false)
{
  gapSymbols.addMember(gapSymbol);
  AlignmentSeq *newTrack=new AlignmentSeq(seqName,0,gapSymbols);
  newTrack->getSeq()=S;
  addTrack(newTrack);
}



MultSeqAlignment::MultSeqAlignment(const MultiAlignment &strAlign,
				   const Alphabet &alphabet,Symbol gapSymbol)
  : alphabet(alphabet),
    phase(strAlign.getPhase()),
    gapSymbol(gapSymbol),
    gapSymbols(alphabet.size()),
    score(1.0),
    annotationEnabled(false), 
    wantAnnoCaps(false)
{
  gapSymbols.addMember(gapSymbol);
  convert(strAlign);
}



MultSeqAlignment::MultSeqAlignment(const MultiAlignment &strAlign,
				   const Alphabet &alphabet,
				   const BitSet &gapSymbols)
  : alphabet(alphabet),
    phase(strAlign.getPhase()),
    gapSymbols(gapSymbols),
    score(1.0),
    annotationEnabled(false), 
    wantAnnoCaps(false)
{
  int N=gapSymbols.getMaxSize();
  for(int i=0 ; i<N ; ++i)
    if(gapSymbols.isMember(i))
      { gapSymbol=i; break; }
  convert(strAlign);
}



MultSeqAlignment::MultSeqAlignment(const Alphabet &alphabet,
				   const BitSet &gapSymbols)
  : alphabet(alphabet),
    gapSymbols(gapSymbols),
    score(1.0),
    annotationEnabled(false), 
    wantAnnoCaps(false)
{
  int N=gapSymbols.getMaxSize();
  for(int i=0 ; i<N ; ++i)
    if(gapSymbols.isMember(i))
      { gapSymbol=i; break; }
}



const BitSet &MultSeqAlignment::getGapSymbols() const 
{
  return gapSymbols;
}



MultSeqAlignment::~MultSeqAlignment()
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i)
    delete tracks[i];
}



void MultSeqAlignment::convert(const MultiAlignment &alignment)
{
  Symbol gap;
  int N=gapSymbols.getMaxSize();
  for(int i=0 ; i<N ; ++i)
    if(gapSymbols.isMember(i))
      { gap=i; break; }

  int n=alignment.getNumTracks();
  for(int i=0 ; i<n ; ++i) {
    const AlignmentTrack &oldTrack=alignment.getIthTrack(i);
    BOOM::String seqStr=oldTrack.getSeq();
    seqStr.toupper();
    BOOM::String name=oldTrack.getName();
    int ID=oldTrack.getID();//oldTrack.getName();
    AlignmentSeq *newTrack=new AlignmentSeq(name,ID,gapSymbols);
    Sequence seq(seqStr,alphabet);
    int L=seq.getLength();
    for(int i=0 ; i<L ; ++i)
      if(seq[i]==INVALID_SYMBOL) seq[i]=gap;
    newTrack->getSeq()=seq;
    if(oldTrack.isTarget()) newTrack->setTarget();
    addTrack(newTrack);
  }
}




void MultSeqAlignment::addTrack(AlignmentSeq *track)
{
  int trackID=tracks.size();
  tracks.push_back(track);
  nameToTrack[track->getName()]=trackID;
  track->setID(trackID);
}



void MultSeqAlignment::releaseTrack(int i)
{
    tracks[i]=NULL;
}



AlignmentSeq &MultSeqAlignment::getTrackByName(const BOOM::String 
					        &trackName)
{
  int trackID=nameToTrack[trackName];
  return *tracks[trackID];
}



AlignmentSeq &MultSeqAlignment::findOrCreateTrack(const BOOM::String &
						  trackName)
{
  if(!nameToTrack.isDefined(trackName))
    {
      int ID=tracks.size();
      nameToTrack[trackName]=ID;
      AlignmentSeq *track=new AlignmentSeq(trackName,ID,gapSymbols);
      addTrack(track);//tracks.push_back(track);
      return *track;
    }
  return *tracks[nameToTrack[trackName]];
}



AlignmentSeq &MultSeqAlignment::getIthTrack(int i)
{
  return *tracks[i];
}



int MultSeqAlignment::getNumTracks() const
{
  return tracks.size();
}



int MultSeqAlignment::getLength() const
{
  int length=0;
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i) {
    int L=tracks[i]->getLength();
    if(L>length) length=L;
  }
  return length;
}



void MultSeqAlignment::deleteSequences()
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i)
      if(tracks[i])
          tracks[i]->deleteSequence();
}



void MultSeqAlignment::save(const BOOM::String &filename,bool alignLeft,
			    bool wantAnno) 
{
  ofstream os(filename.c_str());
  if(!os.good()) throw BOOM::String("Error writing to file: ")+filename;
  save(os,alignLeft,wantAnno);
}



void MultSeqAlignment::save(ostream &os,bool alignLeft,bool wantAnno) 
{
  int length=getLength();
  extendToLength(length);
  printSlice(os,0,length,'+',length,alignLeft,wantAnno);
}



void MultSeqAlignment::capByAnno()
{
  wantAnnoCaps=true;
}



void MultSeqAlignment::printSlice(ostream &os,int begin,int length,
                                  char strand,int lineLength,
				  bool alignLeft,bool wantAnno) const
{
  os<<"a score="<<score<<endl;
  int n=tracks.size();
  int end=begin+length;
  for(int pos=begin ; pos<end ; pos+=lineLength) {
    int L=end-pos;
    if(L>lineLength) L=lineLength;
    if(annotationEnabled) emitAnnotationSlice(os,pos,L);
    for(int i=0 ; i<n ; ++i) {
      AlignmentSeq *track=tracks[i];
      if(!track) continue;
      BOOM::String label=track->getName();
      if(!alignLeft) os<<"s "<<label<<" "<<pos<<" "<<L<<" + . ";
      BOOM::Sequence trackSeq=track->getSeq();
      if(pos<trackSeq.getLength()) {
	BOOM::Sequence subseq;
	trackSeq.getSubsequence(pos,L,subseq);
	String &S=*subseq.toString(alphabet);
	if(strand=='-') S=ProteinTrans::reverseComplement(S);
	if(wantAnnoCaps) {
	  String anno;
	  anno=track->getAnnoTrack().substring(pos,L);
	  S.tolower();
	  int L1=anno.length(), L2=S.length();
	  for(int i=0 ; i<L1 && i<L2 ; ++i) {
	    char a=anno[i];
	    if(a!='.' && a!=' ') S[i]=toupper(S[i]);
	  }
	}
	os<<S;
	delete &S;
      }

      const Sequence &seq=track->getSeq();
      Sequence subseq;
      seq.getSubsequence(0,pos,subseq);
      int nonGap=subseq.getLength()-subseq.countOccurrences(gapSymbol);
      
      //if(alignLeft) os<<" "<<label<<" "<<pos<<" "<<L<<" + . ";
      if(alignLeft) os<<" "<<label<<" "<<nonGap<<" "<<L<<" + . ";

      os<<endl;
      if(wantAnno) {
	String &annoTrack=track->getAnnoTrack();
	String anno;
	if(pos<annoTrack.length())
	  anno=track->getAnnoTrack().substring(pos,L);
	os<<anno<<endl;
      }
    }
    os<<endl;
  }
}



void MultSeqAlignment::emitAnnotationSlice(ostream &os,int begin,int L) const
{
  int end=begin+L;
  int len=annotationTrack.size();
  if(end>len) end=len;
  for(int i=begin ; i<end ; ++i) os<<annotationTrack[i];
  os<<endl;
}



void MultSeqAlignment::enableAnnotation(bool e)
{
  annotationEnabled=e;
}



void MultSeqAlignment::addGapSymbol(Symbol s)
{
  gapSymbols.addMember(s);
}



int MultSeqAlignment::rightmostGapPos(const Sequence &S,
				      const BitSet &gapSymbols) {
  int L=S.getLength();
  for(int i=L-1 ; i>=0 ; --i)
    if(gapSymbols.isMember(S[i])) return i;
  return -1;
}



MultSeqAlignment *MultSeqAlignment::getSlice(int begin,int length)
{
  MultSeqAlignment &A=*new MultSeqAlignment(alphabet,gapSymbols);
  A.nameToTrack=nameToTrack;
  /*
  BOOM::Vector<AlignmentSeq*> tracks;
  BOOM::Map<BOOM::String,int> nameToTrack;
  int length, phase;
   */

  int newBegin=tracks[0]->getBegin()+begin;
  int numTracks=tracks.size();
  for(int i=0 ; i<numTracks ; ++i) {
    AlignmentSeq *oldTrack=tracks[i];
    AlignmentSeq *newTrack=new AlignmentSeq(oldTrack->getName(),
					    oldTrack->getID(),gapSymbols);
    if(oldTrack->isTarget()) newTrack->setTarget();
    Sequence subseq;
    if(oldTrack->getLength()>0)
      oldTrack->getSeq().getSubsequence(begin,length,subseq);
    newTrack->addHSP(newBegin,subseq);
    A.addTrack(newTrack);
  }
  return &A;
}



bool MultSeqAlignment::containsGap()
{
  int nAlpha=alphabet.size();
  BOOM::Vector<AlignmentSeq*>::iterator cur=tracks.begin(), end=tracks.end();
  for(; cur!=end ; ++cur) {
    AlignmentSeq *track=*cur;
    for(Symbol s=0 ; s<nAlpha ; ++s) {
      if(gapSymbols.isMember(s))
	if(track->getSeq().containsSymbol(s)) return true;
    }
  }
  return false;
}




void MultSeqAlignment::deleteTargetGaps(int targetTrackID)
{
  // First, find out how many non-gap columns there are
  int length=tracks[targetTrackID]->getLength();
  AlignmentSeq &targetTrack=*tracks[targetTrackID];
  int nonGaps=0;
  for(int pos=0 ; pos<length ; ++pos)
    if(!gapSymbols.isMember(targetTrack[pos]))
      ++nonGaps;

  // Next, splice the non-target tracks, using the target track's gap
  // patterns as a template
  int n=tracks.size();
  for(int trackID=0 ; trackID<n ; ++trackID) {
    if(trackID==targetTrackID) continue;
    AlignmentSeq &track=*tracks[trackID];
    if(track.getLength()==0) continue;
    AlignmentSeq &newTrack=
      *new AlignmentSeq(track.getName(),trackID,gapSymbols);
    newTrack.extendToLength(nonGaps,gapSymbol);
    int to=0;
    for(int pos=0 ; pos<length ; ++pos)
      {
	if(!gapSymbols.isMember(targetTrack[pos])) {
	  newTrack[to++]=track[pos];
	}	
      }
    tracks[trackID]=&newTrack;
    delete &track;
  }

  // Now splice the target track
  AlignmentSeq &newTrack=
    *new AlignmentSeq(targetTrack.getName(),targetTrackID,gapSymbols);
  newTrack.extendToLength(nonGaps,gapSymbol);
  int to=0;
  for(int pos=0 ; pos<length ; ++pos)
    if(!gapSymbols.isMember(targetTrack[pos]))
      newTrack[to++]=targetTrack[pos];
  tracks[targetTrackID]=&newTrack;
  delete &targetTrack;
}


void MultSeqAlignment::setPhase(int p) 
{
  phase=p;
}



void MultSeqAlignment::append(MultSeqAlignment &other)
{
  int length=getLength();
  int numTracks=other.tracks.size();
  for(int i=0 ; i<numTracks ; ++i) {
    AlignmentSeq *otherTrack=other.tracks[i];
    if(nameToTrack.isDefined(otherTrack->getName())) {
      AlignmentSeq *myTrack=tracks[nameToTrack[otherTrack->getName()]];
      myTrack->extendToLength(length,gapSymbol);
      myTrack->append(*otherTrack);
    }
    else {
      int newID=tracks.size();
      AlignmentSeq *newTrack=
	new AlignmentSeq(otherTrack->getName(),newID,gapSymbols);
      newTrack->extendToLength(length,gapSymbol);
      newTrack->append(*otherTrack);
      nameToTrack[newTrack->getName()]=newID;
      addTrack(newTrack);//tracks.push_back(newTrack);
    }
  }
}



void MultSeqAlignment::append(MultSeqAlignment &other,int begin,int L)
{
  int length=getLength();
  int numTracks=other.tracks.size();
  for(int i=0 ; i<numTracks ; ++i) {
    AlignmentSeq *otherTrack=other.tracks[i];
    if(nameToTrack.isDefined(otherTrack->getName())) {
      AlignmentSeq *myTrack=tracks[nameToTrack[otherTrack->getName()]];
      myTrack->append(*otherTrack,begin,L);
    }
    else {
      int newID=tracks.size();
      AlignmentSeq *newTrack=
	new AlignmentSeq(otherTrack->getName(),newID,gapSymbols);
      newTrack->extendToLength(length,gapSymbol);
      newTrack->append(*otherTrack,begin,L);
      nameToTrack[newTrack->getName()]=newID;
      addTrack(newTrack);
    }
  }
}



void MultSeqAlignment::extendToLength(int length)
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i) {
    tracks[i]->extendToLength(length,gapSymbol);
    tracks[i]->getAnnoTrack().padOrTruncate(length);
  }
}



double MultSeqAlignment::getScore() const
{
  return score;
}



void MultSeqAlignment::setScore(double s)
{
  score=s;
}



bool MultSeqAlignment::trackExists(const String &name)
{
  return nameToTrack.isDefined(name);
}



Array1D<char> &MultSeqAlignment::getAnnotationTrack()
{
  return annotationTrack;
}



void MultSeqAlignment::sortTracksByName()
{
  struct Cmp : public Comparator<AlignmentSeq*> {
    bool equal(AlignmentSeq* &a,AlignmentSeq* &b)
      { return a->getName()==b->getName(); }
    bool greater(AlignmentSeq* &a,AlignmentSeq* &b)
      { return a->getName()>b->getName(); }
    bool less(AlignmentSeq* &a,AlignmentSeq* &b)
      { return a->getName()<b->getName(); }
  } cmp;
  VectorSorter<AlignmentSeq*> sorter(tracks,cmp);
  sorter.sortAscendInPlace();
}


