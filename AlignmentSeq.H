/***********************************************************************
 AlignmentSeq.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_AlignmentSeq_H
#define INCL_AlignmentSeq_H
#include "String.H"
#include "Sequence.H"
#include "BitSet.H"


// a sequence (i.e., "track") in an alignment

namespace BOOM {
class AlignmentSeq 
{
public:
  AlignmentSeq(const BOOM::String &name,int ID,const BitSet &gapSymbols);
  const BOOM::String &getName() const;
  int getID() const;
  void setID(int id) {ID=id;}
  int getBegin() const; // relative to source genome, not to alignment!
  int getLength() const;
  void addHSP(int begin,const BOOM::Sequence &seq);
  void extendToLength(int length,Symbol gapSymbol);
  const BOOM::Sequence &getSeq() const;
  BOOM::Sequence &getSeq();
  inline Symbol &operator[](int);
  void deleteSequence();
  void setTarget();
  bool isTarget() const;
  void append(const AlignmentSeq &);
  void append(const AlignmentSeq &,int begin,int length);
  String &getAnnoTrack();
private:
  BOOM::String name;
  int begin; // relative to source genome, not to alignment!
  BOOM::Sequence seq;
  bool isTargetTrack;
  int ID;
  BitSet gapSymbols;
  String annoTrack;
};
    
 
    
Symbol &AlignmentSeq::operator[](int index)
{
    return seq[index];
}

}

#endif
