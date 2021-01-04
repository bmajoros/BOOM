/****************************************************************
 CigarString.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_CigarString_H
#define INCL_CigarString_H
#include <iostream>
#include "Vector.H"
#include "String.H"
#include "Regex.H"
#include "CigarAlignment.H"
#include "Interval.H"
using namespace std;
namespace BOOM {


/***********************************************************************
 NOTE: This class implements a version of CIGAR strings that is opposite
 that of SAM files, in that insertions (I) and deletions (D) are 
 reversed (i.e., the role of query and reference are reversed).

 When dealing with CIGAR strings from SAM files, use SamCigarString
 instead of this class!
 ***********************************************************************/


/***********************************************************************
                           enum CigarOpType
 ***********************************************************************/
enum CigarOpType {
  CIGAR_MATCH,
  CIGAR_INSERT,
  CIGAR_DELETE,
  CIGAR_SOFT_MASK
};
ostream &operator<<(ostream &,const CigarOpType &);



/***********************************************************************
                            struct CigarOp
 ***********************************************************************/
struct CigarOp {
  CigarOp() {}
  CigarOp(CigarOpType,int);
  CigarOpType type;
  int rep;
  Interval interval1, interval2;

  CigarOpType getOp() { return type; }
  int getLength() { return rep; }
  virtual bool advanceInQuery();
  virtual bool advanceInRef();
  Interval &getQueryInterval() { return interval1; }
  Interval &getRefInterval() { return interval2; }
};
ostream &operator<<(ostream &,const CigarOp &);



/***********************************************************************
                           class CigarString
 ***********************************************************************/
class CigarString {
public:
  CigarString(const String &);
  CigarString() {}
  void load(const String &filename);
  virtual CigarAlignment *getAlignment();
  int length() const;
  CigarOp &operator[](int);
  bool completeMatch();
  Vector<CigarOp> &asVector();
  void printOn(ostream &) const;
  CigarString *unrollMatches() const;
  void reverse() { ops.reverse(); }
  void computeIntervals(int refPos);
protected:
  static Regex re;
  Vector<CigarOp> ops;
  void parse(const String &);
  CigarOpType charToOpType(char);
};
ostream &operator<<(ostream &,const CigarString &);

}
#endif

