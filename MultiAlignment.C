/***********************************************************************
 MultiAlignment.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "MultiAlignment.H"
#include "ProteinTrans.H"
#include <iostream>
#include <fstream>
using namespace std;
using namespace BOOM;


BOOM::Regex MultiAlignment::lineParser(
  "s\\s+(\\S+)\\s+(\\d+)\\s+(\\d+)\\s+(.)\\s+(\\S+)\\s+(\\S+.*)");
BOOM::Regex MultiAlignment::lineParser_empty(
  "s\\s+(\\S+)\\s+(\\d+)\\s+(\\d+)\\s+(.)\\s+(\\S+)\\s*");

BOOM::Regex MultiAlignment::alineParser("a\\s+.*phase\\s*=\\s*(\\d+).*");

BOOM::Map<istream*,int> MultiAlignment::nextPhases;
BOOM::Map<istream*,String> MultiAlignment::nextHeaders;



MultiAlignment::MultiAlignment()
  : phase(0)
{
  // default ctor
}



MultiAlignment::~MultiAlignment()
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i)
    delete tracks[i];
}



void MultiAlignment::loadMAF(const BOOM::String &filename)
{
  ifstream is(filename.c_str());
  if(!is.good()) throw BOOM::String("Can't open file ")+filename;
  loadMAF(is);
}



MultiAlignment *MultiAlignment::nextAlignmentFromMAF(istream &is)
{
  int nextPhase=0;
  if(nextPhases.isDefined(&is)) nextPhase=nextPhases[&is];
  MultiAlignment *maf=new MultiAlignment();
  nextPhases[&is]=maf->loadMAF(is,nextPhase);
  if(maf->getLength()==0) {
    delete maf;
    maf=NULL;
  }
  return maf;
}



void MultiAlignment::addTrack(AlignmentTrack *track)
{
  int trackID=tracks.size();
  tracks.push_back(track);
  nameToTrack[track->getName()]=trackID;
  track->setID(trackID);
}



void MultiAlignment::releaseTrack(int i)
{
    tracks[i]=NULL;
}



int MultiAlignment::loadMAF(istream &is,int passedPhase)
{
  phase=passedPhase;
  int length=0;
  int blockLength=0, nextPhase=0;
  int maxLength=0;
  while(!is.eof())
    {
      BOOM::String line;
      line.getline(is);
      if(is.eof() && line.isEmpty()) break;
      line.trimWhitespace();
      if(line.isEmpty()) 
	{
	  //length+=blockLength;
	  //blockLength=0;
	  continue;
	}

      if(line[0]=='a') {  // && tracks.size()>0) break;
	if(alineParser.match(line)) nextPhase=alineParser[1];
	nextHeaders[&is]=line; 
	if(tracks.size()>0) break;
	header=line;
	phase=nextPhase;
      }

      if(line[0]=='s')
	{
            String more;
            if(line.length()>300)
            {
                more=line.substring(300,line.length()-300);
                line=line.substring(0,300);
            }
            if(lineParser.match(line))
	    {
                BOOM::String name=lineParser[1];
                int begin=lineParser[2].asInt();
                int len=lineParser[3].asInt();
                char strand=lineParser[4][0];
                int contigLength=lineParser[5].asInt();
                BOOM::String seqStr=lineParser[6];
                seqStr+=more;
                //blockLength=seqStr.length();
		int L=seqStr.length();
		if(L>maxLength) maxLength=L;
                AlignmentTrack &track=findOrCreateTrack(name);
                track.extendToLength(length);
                track.addHSP(begin,seqStr);
	    }
            else if(lineParser_empty.match(line)) {}
            else throw BOOM::String("Can't parse line: ")+line;
	}
    }
  //###length+=blockLength;
  length=maxLength;

  // Extend all tracks to the same length
  BOOM::Vector<AlignmentTrack*>::iterator cur=tracks.begin(), 
    end=tracks.end();
  length=0;
  for(; cur!=end ; ++cur) {
    int L=(*cur)->getLength();
    if(L>length) length=L;
  }
  for(; cur!=end ; ++cur)
    (*cur)->extendToLength(length);

  // Get header
  if(header.length()==0) header=nextHeaders[&is];

  // Return the phase of the next alignment
  return nextPhase;
}



AlignmentTrack &MultiAlignment::getTrackByName(const BOOM::String 
					        &trackName)
{
  return *tracks[nameToTrack[trackName]];
}



bool MultiAlignment::trackExists(const String &name)
{
  return nameToTrack.isDefined(name);
}



AlignmentTrack &MultiAlignment::findOrCreateTrack(const BOOM::String &
						  trackName)
{
  if(!nameToTrack.isDefined(trackName))
    {
      int ID=tracks.size();
      nameToTrack[trackName]=ID;
      AlignmentTrack *track=new AlignmentTrack(trackName,ID);
      tracks.push_back(track);
      return *track;
    }
  return *tracks[nameToTrack[trackName]];
}



AlignmentTrack &MultiAlignment::getIthTrack(int i)
{
  return *tracks[i];
}



const AlignmentTrack &MultiAlignment::getIthTrack(int i) const
{
  return *tracks[i];
}



int MultiAlignment::getNumTracks() const
{
  return tracks.size();
}



int MultiAlignment::getLength() const
{
  int length=0;
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i) {
    int len=tracks[i]->getLength();
    if(len>length) length=len;
  }
  return length;
}



void MultiAlignment::printSlice(ostream &os,int begin,int length,
				char strand,int lineLength,
				String extraTags) const
{
  String header=this->header;
  if(header=="") header="a score=0.0";
  os<<header<<" "<<extraTags<<endl;
    int n=tracks.size();
    int end=begin+length;
    for(int pos=begin ; pos<end ; pos+=lineLength)
    {
        int L=end-pos;
        if(L>lineLength) L=lineLength;
        for(int i=0 ; i<n ; ++i)
	{
            AlignmentTrack *track=tracks[i];
            if(!track) continue;
            BOOM::String label=track->getName();
	    if(track->getBegin()<0) continue;
	    if(track->getBegin()+pos<0) 
	      {cout<<track->getBegin()<<" "<<pos<<endl;throw "MultiAlignment";}
            os<<"s "<<label<<" "<<(track->getBegin()+pos)<<" "<<L<<" + . ";
            const BOOM::String &seq=track->getSeq();
            if(pos<seq.length())
            {
                String S=seq.substr(pos,L);
                if(strand=='-') S=ProteinTrans::reverseComplement(S);
                os<<S;
            }
            os<<endl;
	}
    }
}



void MultiAlignment::printOn(ostream &os) const
{
  int length=getLength();
  printSlice(os,0,length,'+',length);
}



ostream &BOOM::operator<<(ostream &os,const MultiAlignment &maf)
{
    maf.printOn(os);
    return os;
}



void MultiAlignment::deleteSequences()
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i)
      if(tracks[i])
          tracks[i]->deleteSequence();
}



void MultiAlignment::loadMAF(const BOOM::String &filename,
			     BOOM::Vector<MultiAlignment*> &into)
{
  ifstream is(filename.c_str());
  if(!is.good()) throw BOOM::String("Can't open file ")+filename;
  while(!is.eof())
    {
      MultiAlignment *alignment=new MultiAlignment;
      alignment->loadMAF(is);
      if(alignment->getNumTracks()>0)
	into.push_back(alignment);
      else 
	delete alignment;
    }
}



MultiAlignment *MultiAlignment::combine(BOOM::Vector<MultiAlignment*> &v,
					bool shouldDelete)
{
  MultiAlignment *a=new MultiAlignment;
  int n=v.size();
  if(n>0) a->header=v[0]->header;
  for(int i=0 ; i<n ; ++i)
    {
      MultiAlignment *m=v[i];
      a->append(*m);
      if(shouldDelete) delete m;
    }
  return a;
}



void MultiAlignment::append(const MultiAlignment &other)
{
  if(header.length()==0) header=other.header;
  int otherLength=other.getLength();
  int length=getLength();
  int m=other.tracks.size();
  for(int i=0 ; i<m ; ++i)
    {
      AlignmentTrack *otherTrack=other.tracks[i];
      const String &trackName=otherTrack->getName();
      AlignmentTrack &track=findOrCreateTrack(trackName);
      track.extendToLength(length);
      track.append(*otherTrack);
    }
}



void MultiAlignment::toupper()
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i)
    {
      AlignmentTrack *track=tracks[i];
      if(track)
          track->toupper();
    }
}




AlignmentTrack *MultiAlignment::findTrack(BOOM::Regex &regex)
{
  int n=tracks.size();
  for(int i=0 ; i<n ; ++i) {
    AlignmentTrack *track=tracks[i];
    if(!track) continue;
    if(regex.match(track->getName())) return track;
  }
  return NULL;
}



void MultiAlignment::deleteTargetGaps(int targetTrackID,char gapSymbol)
{
  // First, find out how many non-gap columns there are
  AlignmentTrack &targetTrack=*tracks[targetTrackID];
  int nonGaps=0;
  int length=getLength();
  for(int pos=0 ; pos<length ; ++pos)
    if(targetTrack[pos]!=gapSymbol)
      ++nonGaps;

  // Next, splice the non-target tracks, using the target track's gap
  // patterns as a template
  int n=tracks.size();
  for(int trackID=0 ; trackID<n ; ++trackID) {
    if(trackID==targetTrackID) continue;
    AlignmentTrack &track=*tracks[trackID];
    if(track.getLength()==0) continue;
    AlignmentTrack &newTrack=
      *new AlignmentTrack(track.getName(),trackID);
    newTrack.extendToLength(nonGaps);
    int to=0;
    for(int pos=0 ; pos<length ; ++pos)
      if(targetTrack[pos]!=gapSymbol)
	newTrack[to++]=track[pos];
    tracks[trackID]=&newTrack;
    delete &track;
  }

  // Now splice the target track
  AlignmentTrack &newTrack=
    *new AlignmentTrack(targetTrack.getName(),targetTrackID);
  newTrack.extendToLength(nonGaps);
  int to=0;
  for(int pos=0 ; pos<length ; ++pos)
    if(targetTrack[pos]!=gapSymbol)
      newTrack[to++]=targetTrack[pos];
  tracks[targetTrackID]=&newTrack;
  delete &targetTrack;

  // Update object fields
  length=nonGaps;
}



void MultiAlignment::ensureAllSameLength()
{
  int n=tracks.size(), L=0;
  for(int i=0 ; i<n ; ++i) {
    AlignmentTrack *track=tracks[i];
    int len=track->getLength();
    if(len>L) L=len;
  }
  for(int i=0 ; i<n ; ++i) {
    AlignmentTrack *track=tracks[i];
    if(track->getLength()<L) track->extendToLength(L);
  }
}


