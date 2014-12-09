/***********************************************************************
 RefAlign.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_RefAlign_H
#define INCL_RefAlign_H
#include "Sequence.H"
#include "String.H"
#include "Alphabet.H"
#include "Vector.H"
#include "SubstitutionMatrix.H"
#include "Alignment.H"
#include "BandedSmithWaterman.H"
#include <iostream>
using namespace std;


/****************************************************************
                       class RefAlignElem
 ****************************************************************/
namespace BOOM {
class RefAlignElem
{
  Alphabet &alphabet;
  const Sequence &sequence;
  Alignment *alignment;
  String track;
  int seqBookmark;       // index into sequence
  int alignmentBookmark; // index into (pairwise) alignment 
public:
  RefAlignElem(Alphabet &,const Sequence &s);
  virtual ~RefAlignElem();
  const Sequence &getSequence() const;
  const String &getTrack() const;
  void emitGap();
  void emitBase();
  MatchType getMatchType() const;
  void advanceAlignmentBookmark();
  void advanceSeqBookmark();
  bool isFullyEmitted() const;
  void setAlignment(Alignment *);
};



/****************************************************************
                         class RefAlign
 ****************************************************************/
template<class FLT_TYP>
class RefAlign
{
  Vector<RefAlignElem*> elements;
  Alphabet &alphabet;
  const SubstitutionMatrix<FLT_TYP> &M;
  FLT_TYP gapOpenPenalty;
  FLT_TYP gapExtendPenalty;
  int bandWidth;

  Alignment *align(const Sequence &,const Sequence &);
  bool needToInsertIntoRef();
  bool finishedEmitting();
public:
  RefAlign(Alphabet &,const SubstitutionMatrix<FLT_TYP> &,
	   FLT_TYP gapOpenPenalty,FLT_TYP gapExtendPenalty,
	   int bandWidth);
  virtual ~RefAlign();
  void reset();
  void addSequence(const Sequence &);
  void performAlignment();
  int numTracks() const;
  const String &getIthTrack(int i) const;
};
}
using namespace BOOM;


/****************************************************************
                     RefAlignElem methods
 ****************************************************************/
RefAlignElem::RefAlignElem(Alphabet &alphabet,const Sequence &s) 
  : alphabet(alphabet),
    sequence(s), 
    alignment(NULL),
    seqBookmark(0),
    alignmentBookmark(0)
{
  // ctor
}



RefAlignElem::~RefAlignElem() 
{
  delete alignment;
}



const Sequence &RefAlignElem::getSequence() const
{
  return sequence;
}



const String &RefAlignElem::getTrack() const
{
  return track;
}



void RefAlignElem::emitGap()
{
  track+='-';
}



void RefAlignElem::emitBase()
{
  track+=alphabet.lookup(sequence[seqBookmark]);
}



MatchType RefAlignElem::getMatchType() const
{
  if(alignmentBookmark<0||alignmentBookmark>=alignment->getAlignmentLength()) throw String("alignmentBookmark=")+alignmentBookmark+", but alignmentLength="+alignment->getAlignmentLength();
  return (*alignment)[alignmentBookmark];
}



void RefAlignElem::advanceAlignmentBookmark()
{
  ++alignmentBookmark;
}



void RefAlignElem::advanceSeqBookmark()
{
  ++seqBookmark;
}



bool RefAlignElem::isFullyEmitted() const
{
  return seqBookmark==sequence.getLength();
}



void RefAlignElem::setAlignment(Alignment *alignment)
{
  this->alignment=alignment;
}



/****************************************************************
                       RefAlign methods
 ****************************************************************/

template<class FLT_TYP>
RefAlign<FLT_TYP>::RefAlign(Alphabet &alphabet,
			    const SubstitutionMatrix<FLT_TYP> &M,
			    FLT_TYP gapOpenPenalty,
			    FLT_TYP gapExtendPenalty,
			    int bandWidth)
  : alphabet(alphabet),
    M(M),
    gapOpenPenalty(gapOpenPenalty),
    gapExtendPenalty(gapExtendPenalty),
    bandWidth(bandWidth)
{
  // ctor
}



template<class FLT_TYP>
RefAlign<FLT_TYP>::~RefAlign()
{
  // dtor
  
  reset();
}



template<class FLT_TYP>
void RefAlign<FLT_TYP>::reset()
{
  int n=elements.size();
  for(int i=0 ; i<n ; ++i)
    delete elements[i];
  elements.clear();
}



template<class FLT_TYP>
void RefAlign<FLT_TYP>::addSequence(const Sequence &sequence)
{
  RefAlignElem *newElem=new RefAlignElem(alphabet,sequence);
  elements.push_back(newElem);
}



template<class FLT_TYP>
Alignment *RefAlign<FLT_TYP>::align(const Sequence &S1,const Sequence &S2)
{
  BandedSmithWaterman<FLT_TYP> aligner(alphabet,S1,S2,M,gapOpenPenalty,
				       gapExtendPenalty,bandWidth);
  return aligner.fullAlignment();
}



template<class FLT_TYP>
bool RefAlign<FLT_TYP>::finishedEmitting()
{
  Vector<RefAlignElem*>::iterator cur=elements.begin(), 
    end=elements.end();
  for(; cur!=end ; ++cur)
    {
      RefAlignElem &elem=**cur;
      if(!elem.isFullyEmitted()) return false;
    }
  return true;
}



template<class FLT_TYP>
bool RefAlign<FLT_TYP>::needToInsertIntoRef()
{
  Vector<RefAlignElem*>::iterator cur=elements.begin(), 
    end=elements.end();
  ++cur;
  for(; cur!=end ; ++cur)
    {
      RefAlignElem &elem=**cur;
      if(!elem.isFullyEmitted() && elem.getMatchType()==SECOND_UNMATCHED) 
	return true;
    }
  return false;

}



template<class FLT_TYP>
void RefAlign<FLT_TYP>::performAlignment()
{
  // First, compute all pairwise alignments with the reference sequence
  int numElements=elements.size();
  if(numElements<1) 
    throw "RefAlign::performAlignment() -- too few sequences to align!";
  RefAlignElem &refElem=*elements[0];
  const Sequence &refSeq=refElem.getSequence();
  for(int i=1 ; i<numElements ; ++i)
    {
      RefAlignElem &elem=*elements[i];
      elem.setAlignment(align(refSeq,elem.getSequence()));
    }

  // Now merge the pairwise alignments into a textual representation of the
  // full multi-alignment
  while(!finishedEmitting())
    {
      // First, handle all SECOND_UNMATCHEDs (insertion in reference seq)
      while(needToInsertIntoRef())
	{
	  refElem.emitGap();
	  for(int i=1 ; i<numElements ; ++i)
	    {
	      RefAlignElem &elem=*elements[i];
	      if(!elem.isFullyEmitted() && 
		 elem.getMatchType()==SECOND_UNMATCHED)
		{
		  elem.emitBase();
		  elem.advanceSeqBookmark();
		  elem.advanceAlignmentBookmark();
		}
	      else
		elem.emitGap();
	    }
	}
      
      // Now handle all MATCHes and FIRST_UNMATCHEDs
      if(!refElem.isFullyEmitted())
	{
	  refElem.emitBase();
	  refElem.advanceSeqBookmark();
	}
      else refElem.emitGap();
      for(int i=1 ; i<numElements ; ++i)
	{
	  RefAlignElem &elem=*elements[i];
	  if(!elem.isFullyEmitted() && elem.getMatchType()==MATCH)
	    {
	      elem.emitBase();
	      elem.advanceSeqBookmark();
	    }
	  else
	    elem.emitGap();
	  elem.advanceAlignmentBookmark();
	}      
    }
}



template<class FLT_TYP>
int RefAlign<FLT_TYP>::numTracks() const
{
  return elements.size();
}



template<class FLT_TYP>
const String &RefAlign<FLT_TYP>::getIthTrack(int i) const
{
  return elements[i]->getTrack();
}



#endif
