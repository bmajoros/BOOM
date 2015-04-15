/***********************************************************************
 Alignment.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "Alignment.H"
#include <iostream>
using namespace BOOM;


int Alignment::MAX_WIDTH=60;



Alignment::Alignment(const Sequence &s1,const Sequence &s2,Alphabet &alpha,
		     double score)
  : s1(s1), s2(s2), alphabet(alpha), score(score)
{
}



const Sequence &Alignment::getFirstSequence() const
{
  return s1;
}



const Sequence &Alignment::getSecondSequence() const
{
  return s2;
}



int Alignment::getAlignmentLength() const
{
  return matchData.size();
}



MatchType Alignment::operator[](int position) const
{
  return matchData[position];
}



Alignment &Alignment::operator+=(MatchType m)
{
  matchData.push_back(m);
  return *this;
}



void Alignment::printOn(ostream &os) const
{
  String topRow, middleRow, bottomRow;

  Vector<MatchType>::const_iterator cur=matchData.begin(), 
    end=matchData.end();
  int index1=0, index2=0;
  for(; cur!=end ; ++cur)
    switch(*cur)
      {
      case MATCH:
	{
	  Symbol symbol1=s1[index1], symbol2=s2[index2];
	  topRow+=alphabet.lookup(symbol1);
	  bottomRow+=alphabet.lookup(symbol2);
	  middleRow+=(symbol1==symbol2 ? '|' : '*');
	  ++index1;
	  ++index2;
	}
	break;
      case FIRST_UNMATCHED:
	topRow+=alphabet.lookup(s1[index1]);
	bottomRow+='-';
	middleRow+=' ';
	++index1;
	break;
      case SECOND_UNMATCHED:
	topRow+='-';
	bottomRow+=alphabet.lookup(s2[index2]);
	middleRow+=' ';
	++index2;
	break;
      }

  int len=getAlignmentLength(), begin=0, subLen=MAX_WIDTH;
  while(begin<len)
    {
      if(begin+subLen>len) subLen=len-begin;
      os << "Query: " << topRow.substring(begin,subLen) << endl;
      os << "       " << middleRow.substring(begin,subLen) << endl;
      os << "Sbjct: " << bottomRow.substring(begin,subLen) << endl;
      begin+=subLen;
      if(begin<len) os << endl;
    }
}



ostream &BOOM::operator<<(ostream &os,BOOM::Alignment const &alignment)
{
  alignment.printOn(os);
  return os;
}



double Alignment::getScore() const
{
  return score;
}



void Alignment::countMismatches(int &mismatches,int &insertions) const
{
  mismatches=insertions=0;
  Vector<MatchType>::const_iterator cur=matchData.begin(), 
    end=matchData.end();
  int index1=0, index2=0;
  for(; cur!=end ; ++cur)
    switch(*cur)
      {
      case MATCH:
	{
	  Symbol symbol1=s1[index1], symbol2=s2[index2];
	  if(!(symbol1==symbol2)) ++mismatches;
	  ++index1;
	  ++index2;
	}
	break;
      case FIRST_UNMATCHED:
	++insertions;
	++index1;
	break;
      case SECOND_UNMATCHED:
	++index2;
	++insertions;
	break;
      }
}



int Alignment::countNearMatches(SubstitutionMatrix<float> &M)
{
  int count=0;
  Vector<MatchType>::const_iterator cur=matchData.begin(), 
    end=matchData.end();
  int index1=0, index2=0;
  for(; cur!=end ; ++cur)
    switch(*cur)
      {
      case MATCH:
	{
	  Symbol symbol1=s1[index1], symbol2=s2[index2];
	  if(M(symbol1,symbol2)>0) ++count;
	  ++index1;
	  ++index2;
	}
	break;
      case FIRST_UNMATCHED:
	++index1;
	break;
      case SECOND_UNMATCHED:
	++index2;
	break;
      }
  return count;
}



int Alignment::countNearMatches(SubstitutionMatrix<double> &M)
{
  int count=0;
  Vector<MatchType>::const_iterator cur=matchData.begin(), 
    end=matchData.end();
  int index1=0, index2=0;
  for(; cur!=end ; ++cur)
    switch(*cur)
      {
      case MATCH:
	{
	  Symbol symbol1=s1[index1], symbol2=s2[index2];
	  if(M(symbol1,symbol2)>0) ++count;
	  ++index1;
	  ++index2;
	}
	break;
      case FIRST_UNMATCHED:
	++index1;
	break;
      case SECOND_UNMATCHED:
	++index2;
	break;
      }
  return count;
}



void Alignment::getResidualsOnRight(Sequence &seq1,Sequence &seq2)
{
  seq1.clear();
  seq2.clear();
  int len=matchData.size();
  int index1=s1.getLength()-1, index2=s2.getLength()-1;
  for(int i=len-1 ; i>0 ; --i)
    switch(matchData[i])
      {
      case MATCH:
	return;
      case FIRST_UNMATCHED:
	seq1.prepend(s1[index1]);
	--index1;
	break;
      case SECOND_UNMATCHED:
	seq2.prepend(s2[index2]);
	--index2;
      }
}



void Alignment::render(String &str1,String &str2)
{
  Vector<MatchType>::const_iterator cur=matchData.begin(), 
    end=matchData.end();
  int index1=0, index2=0;
  for(; cur!=end ; ++cur)
    switch(*cur)
      {
      case MATCH:
	{
	  Symbol symbol1=s1[index1], symbol2=s2[index2];
	  str1+=alphabet.lookup(symbol1);
	  str2+=alphabet.lookup(symbol2);
	  ++index1;
	  ++index2;
	}
	break;
      case FIRST_UNMATCHED:
	{
	  Symbol symbol1=s1[index1];
	  str1+=alphabet.lookup(symbol1);
	  str2+='-';
	  ++index1;
	}
	break;
      case SECOND_UNMATCHED:
	{
	  Symbol symbol2=s2[index2];
	  str1+='-';
	  str2+=alphabet.lookup(symbol2);
	  ++index2;
	}
	break;
      }
}


char matchTypeToCigarLetter(MatchType m)
{
  switch(m)
    {
    case MATCH: return 'M';
    case FIRST_UNMATCHED: return 'D';
    case SECOND_UNMATCHED: return 'I';
    default: INTERNAL_ERROR;
    }
}



String Alignment::getCigarString() const
{
  int L=matchData.size();
  if(L==0) return "";
  String cigar;
  int opLen=1;
  MatchType matchType=matchData[0];
  char op=matchTypeToCigarLetter(matchType);
  for(int i=1 ; i<L ; ) {
    while(matchData[i]==matchType && i<L) { ++opLen; ++i; }
    cigar+=String(opLen); cigar+=String(op);
    if(i<L) { 
      matchType=matchData[i]; 
      op=matchTypeToCigarLetter(matchType); 
      opLen=1; 
      ++i; }
  }
  return cigar;
}


