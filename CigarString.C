/****************************************************************
 CigarString.C
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "CigarString.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;

Regex CigarString::re("^(\\d+)(\\D)(.*)");


ostream &BOOM::operator<<(ostream &os,const CigarOpType &t)
{
  switch(t)
    {
    case CIGAR_MATCH: os<<"M";
    case CIGAR_INSERT:os<<"I";
    case CIGAR_DELETE:os<<"D";
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



ostream &operator<<(ostream &os,const CigarString &S)
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
    case 'M': return CIGAR_MATCH;
    case 'I': return CIGAR_INSERT;
    case 'D': return CIGAR_DELETE;
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
  for(const Vector<CigarOp>::iterator cur=ops.begin(), end=ops.end() ; 
      cur!=end ; ++cur) {
    CigarOp op=*cur;
    if(op.type!=CIGAR_INSERT) ++len;
  }
  CigarAlignment &A=*new CigarAlignment(len);
  int fromPos=0, toPos=0;
  for(const Vector<CigarOp>::iterator cur=ops.begin(), end=ops.end() ; 
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
      }
  }
  return &A;
}



