/****************************************************************
 CigarString.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <fstream>
#include "CigarString.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;

Regex CigarString::re("^(\\d+)(\\D)(.*)");



ostream &BOOM::operator<<(ostream &os,const CigarOpType &t)
{
  switch(t)
    {
    case CIGAR_MATCH: os<<"M"; break;
    case CIGAR_INSERT:os<<"I"; break;
    case CIGAR_DELETE:os<<"D"; break;
    case CIGAR_SOFT_MASK:os<<"S"; break;
    default: INTERNAL_ERROR;
    }
  return os;
}



ostream &BOOM::operator<<(ostream &os,const CigarOp &op)
{
  os<<op.rep;
  os<<op.type;
  return os;
}



ostream &BOOM::operator<<(ostream &os,const CigarString &S)
{
  S.printOn(os);
  return os;
}



CigarOp::CigarOp(CigarOpType t,int r)
  : type(t), rep(r)
{
  // ctor
}



CigarString::CigarString(const String &S)
{
  parse(S);
}



int CigarString::length() const
{
  return ops.size();
}



CigarOp &CigarString::operator[](int i)
{
  return ops[i];
}



Vector<CigarOp> &CigarString::asVector()
{
  return ops;
}



void CigarString::parse(const String &S)
{
  ops.clear();
  String local=S;
  local.trimWhitespace();
  while(!local.isEmpty()) {
    if(!re.match(local)) throw S+" : cannot parse CIGAR string";
    String repStr=re[1], opStr=re[2], rest=re[3];
    int rep=repStr.asInt();
    char opChar=opStr[0];
    CigarOpType type=charToOpType(opChar);
    ops.push_back(CigarOp(type,rep));
    local=rest;
  }
}



CigarOpType CigarString::charToOpType(char c)
{
  switch(c)
    {
    case 'M': case '=': case 'X': return CIGAR_MATCH;
    case 'I': return CIGAR_INSERT;
    case 'D': case 'N': return CIGAR_DELETE;
    case 'S': return CIGAR_SOFT_MASK;
    default: throw String("unrecognized CIGAR operator: ")+c;
    }
}



void CigarString::printOn(ostream &os) const
{
  for(Vector<CigarOp>::const_iterator cur=ops.begin(), end=ops.end() ; 
      cur!=end ; ++cur)
    os<<*cur;
}



CigarAlignment *CigarString::getAlignment()
{
  int len=0;
  for(Vector<CigarOp>::const_iterator cur=ops.begin(), end=ops.end() ; 
      cur!=end ; ++cur) {
    CigarOp op=*cur;
    if(op.type!=CIGAR_INSERT && op.type!=CIGAR_SOFT_MASK) len+=op.rep;
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
      case CIGAR_DELETE:
	for(int i=0 ; i<op.rep ; ++i) A[fromPos++]=CIGAR_UNDEFINED;
	break;
      case CIGAR_INSERT:
	toPos+=op.rep;
	break;
      case CIGAR_SOFT_MASK:
	toPos+=op.rep;
	//throw RootException("soft-masking not implemented in CigarString::getAlignment()");
	break;
      }
  }
  return &A;
}



void CigarString::load(const String &filename)
{
  ifstream is(filename.c_str());
  String line;
  is>>line;
  parse(line);
}



CigarString *CigarString::unrollMatches() const
{
  CigarString *unrolled=new CigarString();
  CigarOp match(CIGAR_MATCH,1);
  for(Vector<CigarOp>::const_iterator cur=ops.begin(), end=ops.end() ;
      cur!=end ; ++cur) {
    CigarOp op=*cur;
    if(op.type==CIGAR_MATCH)
      for(int i=0 ; i<op.rep ; ++i) unrolled->ops.push_back(match);
    else unrolled->ops.push_back(op);
  }
  return unrolled;
}



bool CigarOp::advanceInQuery()
{
  switch(type) {
  case CIGAR_MATCH:
  case CIGAR_INSERT:
  case CIGAR_SOFT_MASK:
    return true;
  }
  return false;
}



bool CigarOp::advanceInRef()
{
  switch(type) {
  case CIGAR_MATCH:
  case CIGAR_DELETE:
    return true;
  }
  return false;
}



void CigarString::computeIntervals(int refPos)
{
  const int n=ops.size();
  int begin1=0, begin2=refPos;
  for(int i=0 ; i<n ; ++i) {
      CigarOp &op=ops[i];
      const int L=op.getLength();
      int end1=begin1, end2=begin2;
      if(op.advanceInQuery()) end1+=L;
      if(op.advanceInRef()) end2+=L;
      op.interval1=Interval(begin1,end1);
      op.interval2=Interval(begin2,end2);
      begin1=end1; begin2=end2;
    }
}



bool CigarString::completeMatch()
{
  return ops.size()==1 && ops[0].getOp()==CIGAR_MATCH;
}


