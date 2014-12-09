/***********************************************************************
 RapidSequenceComparison.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_RapidSequenceComparison_H
#define INCL_RapidSequenceComparison_H
#include "SubstitutionMatrix.H"
#include "Sequence.H"

namespace BOOM {
template<class FLT_DBL>
class RapidSequenceComparison
{
  Alphabet &alphabet;
  SubstitutionMatrix<FLT_DBL> &M;

  inline void updateCounts(float &lengthSum,
			   int &count,float &length,FLT_DBL);
public:
  RapidSequenceComparison(Alphabet &,SubstitutionMatrix<FLT_DBL> &);
  FLT_DBL compare(const Sequence &,const Sequence &);
};
}
using namespace BOOM;


template<class FLT_DBL>
RapidSequenceComparison<FLT_DBL>::RapidSequenceComparison(
						     Alphabet &alphabet,
					 SubstitutionMatrix<FLT_DBL> &M)
  : alphabet(alphabet),
    M(M)
{
}



template<class FLT_DBL>
void RapidSequenceComparison<FLT_DBL>::updateCounts(float &lengthSum,
						    int &count,
						    float &matchLength,
						    FLT_DBL Mval)
{
  if(Mval>0) matchLength+=Mval;
  else 
    {
      lengthSum+=matchLength*matchLength;
      ++count;
      matchLength=0;
    }
}



template<class FLT_DBL>
FLT_DBL RapidSequenceComparison<FLT_DBL>::compare(const Sequence &s1,
						  const Sequence &s2)
{
  float lengthSum=0;
  int count=0;
  int n1=s1.getLength(), n2=s2.getLength();
  int n=min(n1,n2);
  for(int x0=0 ; x0<n ; ++x0)
    {
      float matchLength=0;
      for(int y=0, x=x0 ; x<n ; ++x, ++y)
	{
	  updateCounts(lengthSum,count,matchLength,M(s1[x],s2[y]));
	}
      lengthSum+=matchLength*matchLength;
      ++count;
    }
  for(int y0=1 ; y0<n ; ++y0)
    {
      float matchLength=0;
      for(int x=0, y=y0 ; y<n ; ++y, ++x)
	{
	  updateCounts(lengthSum,count,matchLength,M(s1[x],s2[y]));
	}
      lengthSum+=matchLength*matchLength;
      ++count;
    }
  FLT_DBL score=lengthSum/static_cast<FLT_DBL>(count);
  return sqrt(score);
}

#endif
