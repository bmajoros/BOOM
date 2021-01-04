/****************************************************************
 SamCigarString.H
 Copyright (C)2021 William H. Majoros (bmajoros@alumni.duke.edu).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_SamCigarString_H
#define INCL_SamCigarString_H
#include <iostream>
#include "CigarString.H"
using namespace std;
namespace BOOM {


/***********************************************************************
                          struct SamCigarOp
 ***********************************************************************/
/*
 interval1 : Interval (in sequence 1 = query)
 interval2 : Interval (in sequence 2 = reference)
 op : M(or =/X)/I/D/S:
                                                            consumes
                                                            query ref
   M 0 alignment match (can be a sequence match or mismatch) yes yes
   I 1 insertion to the reference                            yes  no
   D 2 deletion from the reference                            no yes
   N 3 skipped region from the reference                      no yes
   S 4 soft clipping (clipped sequences present in SEQ)      yes  no
   H 5 hard clipping (clipped sequences NOT present in SEQ)   no  no
   P 6 padding (silent deletion from padded reference)        no  no
   = 7 sequence match                                        yes yes
   X 8 sequence mismatch                                     yes yes
*/
struct SamCigarOp : public CigarOp {
  SamCigarOp() {}
  SamCigarOp(CigarOpType,int);

  virtual bool advanceInQuery();
  virtual bool advanceInRef();
};



/***********************************************************************
                           class SamCigarString
 ***********************************************************************/
class SamCigarString : public CigarString {
public:
  SamCigarString(const String &);
  SamCigarString() {}
  virtual CigarAlignment *getAlignment();
  //void computeIntervals(int refPos);
};

}
#endif

