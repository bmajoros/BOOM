/****************************************************************
 SamCigarString.C
 Copyright (C)2021 William H. Majoros (bmajoros@alumni.duke.edu).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <fstream>
#include "SamCigarString.H"
using namespace std;
using namespace BOOM;



SamCigarOp::SamCigarOp(CigarOpType t,int r)
  : CigarOp(t,r)
{
  // ctor
}



SamCigarString::SamCigarString(const String &s)
  : CigarString(s)
{
  // ctor
}



CigarAlignment *SamCigarString::getAlignment()
{
  int len=0;
  for(Vector<CigarOp>::const_iterator cur=ops.begin(), end=ops.end() ; 
      cur!=end ; ++cur) {
    CigarOp op=*cur;
    if(op.type!=CIGAR_DELETE) len+=op.rep;
  }
  CigarAlignment &A=*new CigarAlignment(len);
  int fromPos=0, toPos=0;
  for(Vector<CigarOp>::const_iterator cur=ops.begin(), end=ops.end() ; 
      cur!=end ; ++cur) {
    CigarOp op=*cur;
    switch(op.type)
      {
      case CIGAR_MATCH:
	for(int i=0 ; i<op.rep ; ++i) A[fromPos++]=toPos++;
	break;
      case CIGAR_INSERT:
      case CIGAR_SOFT_MASK: // same as insert
	for(int i=0 ; i<op.rep ; ++i) A[fromPos++]=CIGAR_UNDEFINED;
	break;
      case CIGAR_DELETE:
	toPos+=op.rep;
	break;
      }
  }
  return &A;
}











