/***********************************************************************
 GffFeature.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "StrTokenizer.H"
#include "GffFeature.H"
using namespace std;


BOOM::Regex BOOM::GffFeature::transgrpRegex("transgrp=([^;]+)");


BOOM::GffFeature::GffFeature(const BOOM::String &rawLine,
			       const BOOM::String &substrate,
			       const BOOM::String &source,
			       const BOOM::String &featureType,
			       int begin,int end,double score,bool hasScore,
			       char strand,int frame,bool hasFrame)
  : rawLine(rawLine), substrate(substrate), source(source), 
    featureType(featureType), begin(begin), end(end), score(score),
    hasScore(hasScore), strand(strand), frame(frame), hasFrame(hasFrame)
{
}



BOOM::GffFeature::GffFeature(const BOOM::String &rawLine)
  : rawLine(rawLine), score(0), frame(0)
{
  parseLine(rawLine);
}



ostream &operator<<(ostream &os,BOOM::GffFeature &feature)
{
  os<<feature.toGff();
  return os;
}



BOOM::String BOOM::GffFeature::toGff() const
{
  BOOM::String gff=
    substrate+"\t"+source+"\t"+featureType+"\t";
  gff+=BOOM::String(begin+1)+"\t"+BOOM::String(end)+"\t";
  if(hasScore) gff+=BOOM::String("")+score; else gff+=".";
  gff+=BOOM::String("\t")+strand+"\t";
  if(hasFrame) gff+=BOOM::String("")+frame; else gff+=".";
  if(extraFields.size()>0) {
    gff+="\t";
    for(int i=0 ; i<extraFields.size() ; ++i)
      gff+=extraFields[i]+";";
  }
  gff+="\n";
  return gff;
}



BOOM::Vector<BOOM::String> &BOOM::GffFeature::getExtraFields()
{
  return extraFields;
}



bool BOOM::GffFeature::hasExtraFields() const
{
  return extraFields.size()>0;
}



bool BOOM::GffFeature::isFramed() const
  {
    return hasFrame;
  }



bool BOOM::GffFeature::isScored() const
  {
    return hasScore;
  }



bool BOOM::GffFeature::isStranded() const
  {
    return strand!='.';
  }



char BOOM::GffFeature::getStrand() const
  {
    return strand;
  }



const BOOM::String &BOOM::GffFeature::getFeatureType() const
{
  return featureType;
}



const BOOM::String &BOOM::GffFeature::getRawField(int index)
{
  return allFields[index];
}



const BOOM::String &BOOM::GffFeature::getRawLine() const
{
  return rawLine;
}



const BOOM::String &BOOM::GffFeature::getSource() const
{
  return source;
}



const BOOM::String &BOOM::GffFeature::getSubstrate() const
{
  return substrate;
}



double BOOM::GffFeature::getScore() const
{
  return score;
}



int BOOM::GffFeature::getBegin() const
{
   
  return begin;
}



int BOOM::GffFeature::getEnd() const
{
   
  return end;
}



int BOOM::GffFeature::getFrame() const
  {
    return frame;
  }



void BOOM::GffFeature::parseLine(const BOOM::String &line)
{
  BOOM::Vector<BOOM::String> extra;
  BOOM::StrTokenizer tokenizer(line);
  int fieldNum=0;
  BOOM::String dot=".";
  for(; tokenizer.hasMoreTokens() ; ++fieldNum)
    {
      const char *token=tokenizer.nextToken();
      allFields.push_back(token);
      switch(fieldNum)
	{
	case 0:
	  substrate=token;
	  break;
	case 1:
	  source=token;
	  break;
	case 2:
	  featureType=token;
	  break;
	case 3:
	  begin=atoi(token)-1;
	  //cerr<<"reader: begin="<<begin<<endl;
	  break;
	case 4:
	  end=atoi(token);
	  //cerr<<"reader: end="<<end<<endl;
	  //cerr<<line<<endl;
	  break;
	case 5:
	  if(dot==token)
	    hasScore=false;
	  else
	    {
	      hasScore=true;
	      score=atof(token);
	    }
	  break;
	case 6:
	  strand=*token;
	  break;
	case 7:
	  if(dot==token)
	    hasFrame=false;
	  else
	    {
	      hasFrame=true;
	      frame=atoi(token);
	    }
	  break;
	default:
	  extra.push_back(token);
	}
    }

  // Now coalesce the extra fields, keeping separate only those
  // elements separated by a comma (as per the official GFF definition)
  int n=extra.size();
  bool init=true;
  for(int i=0 ; i<n ; ++i)
    {
      BOOM::String field=extra[i];
      if(init)
	{
	  extraFields.push_back(field);
	  init=false;
	}
      else if(field==";") init=false;
      else extraFields[extraFields.size()-1]+=(BOOM::String(" ")+field);
    }
}



int BOOM::GffFeature::getTranscriptId() const
{
  int n=extraFields.size();
  for(int i=0 ; i<n ; ++i)
    {
      const BOOM::String &field=extraFields[i];
      if(transgrpRegex.search(field)) 
	return transgrpRegex[1];
    }
  return -1;
}



int BOOM::GffFeature::overlapBases(const GffFeature &other) const
{
  int b=max(begin,other.begin), e=min(end,other.end);
  int o=e-b;//+1;
  return o>=0 ? o : 0;
}



int BOOM::GffFeature::length() const
{
  return end-begin;//+1;
}



bool GffFeatureComparator::less(BOOM::GffFeature *&a,BOOM::GffFeature *&b)
{
    return a->getBegin()<b->getBegin();
}


    
bool GffFeatureComparator::greater(BOOM::GffFeature *&a,BOOM::GffFeature *&b)
{
    return a->getBegin()>b->getBegin();
}



bool GffFeatureComparator::equal(BOOM::GffFeature *&a,BOOM::GffFeature *&b)
{
    return a->getBegin()==b->getBegin();
}



