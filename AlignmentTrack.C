/***********************************************************************
 AlignmentTrack.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "AlignmentTrack.H"
#include <iostream>
using namespace BOOM;


AlignmentTrack::AlignmentTrack(const BOOM::String &name,int ID)
  : name(name),
    isTargetTrack(false),
    ID(ID),
    begin(0)
{
  // ctor
}



void AlignmentTrack::setTarget()
{
  isTargetTrack=true;
}



bool AlignmentTrack::isTarget() const
{
  return isTargetTrack;
}



const BOOM::String &AlignmentTrack::getName() const
{
  return name;
}



int AlignmentTrack::getBegin() const
{
  return begin;
}



int AlignmentTrack::getLength() const
{
  return seqStr.length();
}



void AlignmentTrack::addHSP(int begin,const BOOM::String &hsp)
{
  if(seqStr.length()==0) this->begin=begin;
  seqStr+=hsp;
}



void AlignmentTrack::extendToLength(int length)
{
  int L=seqStr.length();
  if(L<length)
    {
      int delta=length-L;
      BOOM::String padding(delta,'-');
      seqStr+=padding;
    }
}



const BOOM::String &AlignmentTrack::getSeq() const
{
  return seqStr;
}



void AlignmentTrack::deleteSequence()
{
  seqStr="";
}



int AlignmentTrack::getID() const
{
  return ID;
}



void AlignmentTrack::append(const AlignmentTrack &other)
{
  seqStr+=other.seqStr;
}



void AlignmentTrack::toupper()
{
  seqStr.toupper();
}



int AlignmentTrack::getNonGapLength() const
{
    int l=0, L=seqStr.length();
    for(int i=0 ; i<L ; ++i)
        if(seqStr[i]!='-')
            ++l;
    return l;
}



int AlignmentTrack::mapUngappedCoordToGapped(int ungappedCoord)
{
  int l=0, L=seqStr.length();
  for(int i=0 ; i<L ; ++i) {
    if(seqStr[i]!='-') ++l;
    if(l==ungappedCoord) return i;
  }
  return -1;
}


