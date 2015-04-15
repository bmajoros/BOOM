/***********************************************************************
 Alignment.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
************************************************************************/
#ifndef INCL_Alignment_H
#define INCL_Alignment_H
#include "Sequence.H"
#include <iostream>
#include "MatchType.H"
#include "SubstitutionMatrix.H"


/***********************************************************************
 Represents an alignment between two sequences (nucleotide, amino
 acid, or other).  The alignment is represented by a list of
 match classifications (MATCH/INSERTION/DELETION) that describe
 the individual points on the path through an alignment matrix 
 (i.e., DIAGONAL/UP/RIGHT).  This is a (relatively) compact 
 representation that is bounded by the sum of the two sequence 
 lengths, but it does not give constant time access to a random 
 base.  It is time-efficient only when processing the entire 
 alignment from left to right.  A constant-time random-access 
 alignment representation could be derived from this.
************************************************************************/

namespace BOOM {
class Alignment
{
public:
  static int MAX_WIDTH;
  Alignment(const Sequence &,const Sequence &,Alphabet &,double score=0);
  const Sequence &getFirstSequence() const;
  const Sequence &getSecondSequence() const;
  int getAlignmentLength() const;
  MatchType operator[](int position) const;
  Alignment &operator+=(MatchType);
  double getScore() const;
  void countMismatches(int &mismatches,int &insertions) const;
  int countNearMatches(SubstitutionMatrix<float> &);
  int countNearMatches(SubstitutionMatrix<double> &);
  void getResidualsOnRight(Sequence &,Sequence &);
  void render(String &,String &);
  void printOn(ostream &) const;
  String getCigarString() const;
private:
  const Sequence &s1, &s2;
  Vector<MatchType> matchData;
  double score;
  Alphabet &alphabet;
  friend ostream &operator<<(ostream &,const Alignment &);
};
  ostream &operator<<(ostream &,BOOM::Alignment const &);
}

#endif
