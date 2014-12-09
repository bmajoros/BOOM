/***********************************************************************
 AlignmentSeq.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "AlignmentSeq.H"
#include <iostream>
using namespace BOOM;


AlignmentSeq::AlignmentSeq(const BOOM::String &name,int ID,
			   const BitSet &gapSymbols)
  : name(name),
    isTargetTrack(false),
    ID(ID),
    gapSymbols(gapSymbols)
{
  // ctor
}



void AlignmentSeq::setTarget()
{
  isTargetTrack=true;
}



bool AlignmentSeq::isTarget() const
{
  return isTargetTrack;
}



const BOOM::String &AlignmentSeq::getName() const
{
  return name;
}



int AlignmentSeq::getBegin() const
{
  return begin;
}



int AlignmentSeq::getLength() const
{
  return seq.getLength();
}



void AlignmentSeq::addHSP(int begin,const BOOM::Sequence &hsp)
{
  if(seq.getLength()==0) this->begin=begin;
  seq.append(hsp);
}



void AlignmentSeq::extendToLength(int length,Symbol gapSymbol)
{
  /*
  Symbol gapSymbol;
  int n=gapSymbols.getMaxSize();
  for(int i=0 ; i<n ; ++i)
    if(gapSymbols.isMember(i))
      { gapSymbol=i; break; }
  */

  int L=seq.getLength();
  if(L<length)
    {
      int delta=length-L;
      for(int i=0 ; i<delta ; ++i)
	seq.append(gapSymbol);
    }
}



const BOOM::Sequence &AlignmentSeq::getSeq() const
{
  return seq;
}



BOOM::Sequence &AlignmentSeq::getSeq() {
  return seq;
}



void AlignmentSeq::deleteSequence()
{
  seq.clear();
}



int AlignmentSeq::getID() const
{
  return ID;
}



void AlignmentSeq::append(const AlignmentSeq &other)
{
  seq.append(other.seq);
}



void AlignmentSeq::append(const AlignmentSeq &other,int begin,int L)
{
  Sequence extra;
  other.seq.getSubsequence(begin,L,extra);
  seq.append(extra);
}



String &AlignmentSeq::getAnnoTrack()
{
  return annoTrack;
}






