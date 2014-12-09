/***********************************************************************
 SequenceEntropy.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "SequenceEntropy.H"
#include <iostream>
#include <math.h>
#include "StringMap.H"
#include "Vector.H"
#include "DnaAlphabet.H"
using namespace BOOM;

double logOf2=log(2.0);
inline double lg(double x) {return log(x)/logOf2;}


inline int hashTableSize(int order)
{
  // these are all primes

  switch(order)
    {
    case 0: return 11; break;
    case 1: return 29; break;
    case 2: return 127; break;
    case 3: return 619; break;
    default:
    case 4: return 3121; break;
    }
}



double BOOM::SequenceEntropy::entropy(const Sequence &seq,
				    double &maxEntropy)
{
  BOOM::String *str=seq.toString(DnaAlphabet::global());
  double H=entropy(*str,maxEntropy);
  delete str;
  return H;
}



double BOOM::SequenceEntropy::entropy(const BOOM::String &str,
				    double &maxEntropy)
{
  int len=str.length();
  BOOM::StringMap<int> counts(hashTableSize(0));
  const char *p=str.c_str();
  int total=0;
  for(int i=0 ; i<len ; ++i, ++p)
    {
      if(counts.isDefined(p,1)) 
	++counts.lookup(p,1);
      else 
	counts.lookup(p,1)=1;
      ++total;
    }
  double entropy=0;
  StringMapIterator<int> cur=counts.begin(), end=counts.end();
  for(; cur!=end ; ++cur)
    {
      int count=(*cur).second;
      double p=count/double(total);
      entropy-=p*lg(p);
    }
  maxEntropy=-lg(1.0/counts.size());
  if(entropy>maxEntropy) entropy=maxEntropy;
  return entropy;
}



double BOOM::SequenceEntropy::jointEntropy(const Sequence &seq,int order,
				    double &maxEntropy)
{
  BOOM::String *str=seq.toString(DnaAlphabet::global());
  double H=jointEntropy(*str,order,maxEntropy);
  delete str;
  return H;
}



double BOOM::SequenceEntropy::jointEntropy(const BOOM::String &str,
					   int order,
					   double &maxEntropy)
{
  int len=str.length();
  int gramSize=order+1;
  if(gramSize>=len) 
    throw BOOM::String("Order ")+order+
      " is too large for sequence of length "+len;
  int numWindows=len-gramSize+1;
  BOOM::StringMap<int> counts(hashTableSize(order));
  const char *p=str.c_str();
  int total=0;
  for(int i=0 ; i<numWindows ; ++i, ++p)
    {
      if(counts.isDefined(p,gramSize)) 
	++counts.lookup(p,gramSize);
      else 
	counts.lookup(p,gramSize)=1;
      ++total;
    }
  double entropy=0;
  StringMapIterator<int> cur=counts.begin(), end=counts.end();
  for(; cur!=end ; ++cur)
    {
      int count=(*cur).second;
      double p=count/double(total);
      entropy-=p*lg(p);
    }
  maxEntropy=-lg(1.0/counts.size());
  if(entropy>maxEntropy) entropy=maxEntropy;
  return entropy;
}



double BOOM::SequenceEntropy::conditionalEntropy(const Sequence &seq,
						 int order)
{
  BOOM::String *str=seq.toString(DnaAlphabet::global());
  double H=conditionalEntropy(*str,order);
  delete str;
  return H;
}



double BOOM::SequenceEntropy::conditionalEntropy(const BOOM::String &str,
						 int order)
{
  if(order<1) 
    throw "BOOM::SequenceEntropy::conditionalEntropy() : order<1";
  int len=str.length();
  int gramSize=order+1;
  if(gramSize>=len) 
    throw BOOM::String("Order ")+order+
      " is too large for sequence of length "+len;
  int numWindows=len-gramSize+1;
  BOOM::StringMap<int> counts(hashTableSize(order));
  BOOM::StringMap<int> prefixCounts(hashTableSize(order-1));
  const char *p=str.c_str();
  int total=0;
  for(int i=0 ; i<numWindows ; ++i, ++p)
    {
      if(counts.isDefined(p,gramSize)) 
	++counts.lookup(p,gramSize);
      else 
	counts.lookup(p,gramSize)=1;
      if(prefixCounts.isDefined(p,gramSize-1)) 
	++prefixCounts.lookup(p,gramSize-1);
      else 
	prefixCounts.lookup(p,gramSize-1)=1;
      ++total;
    }
  double entropy=0;
  StringMapIterator<int> cur=counts.begin(), end=counts.end();
  for(; cur!=end ; ++cur)
    {
      int count=(*cur).second;
      const char *s=(*cur).first;
      double p=count/double(total);
      double condP=count/double(prefixCounts.lookup(s,gramSize-1));
      entropy-=p*lg(condP);
    }
  return entropy;
}

