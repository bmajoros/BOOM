/***********************************************************************
 AlignmentTrack.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_AlignmentTrack_H
#define INCL_AlignmentTrack_H
#include "String.H"

namespace BOOM {
class AlignmentTrack
{
public:
  AlignmentTrack(const BOOM::String &name,int ID);
  const BOOM::String &getName() const;
  void setName(const BOOM::String &n) {name=n;}
  void rename(const BOOM::String &n) {name=n;}
  int getID() const;
  void setID(int id) {ID=id;}
  int getBegin() const; // relative to source genome, not to alignment!
  int getLength() const;
  int getNonGapLength() const; // number of non-gap entries in track
  void addHSP(int begin,const BOOM::String &seqStr);
  void extendToLength(int length);
  const BOOM::String &getSeq() const;
  inline char &operator[](int);
  void deleteSequence();
  void setTarget();
  bool isTarget() const;
  void append(const AlignmentTrack &);
  void toupper();
  int mapUngappedCoordToGapped(int ungappedCoord);
private:
  BOOM::String name;
  int begin; // relative to source genome, not to alignment!
  BOOM::String seqStr;
  bool isTargetTrack;
  int ID;
};



char &AlignmentTrack::operator[](int index)
{
    return seqStr[index];
}

}

#endif
