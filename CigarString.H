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
using namespace std;
namespace BOOM {


/***********************************************************************
                           enum CigarOpType
 ***********************************************************************/
enum CigarOpType {
  CIGAR_MATCH,
  CIGAR_INSERT,
  CIGAR_DELETE
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
  CigarAlignment *getAlignment();
  int length() const;
  CigarOp &operator[](int);
  Vector<CigarOp> &asVector();
  void printOn(ostream &) const;
  CigarString *unrollMatches() const;
  void reverse() { ops.reverse(); }
private:
  static Regex re;
  Vector<CigarOp> ops;
  void parse(const String &);
  CigarOpType charToOpType(char);
};
ostream &operator<<(ostream &,const CigarString &);

}
#endif

