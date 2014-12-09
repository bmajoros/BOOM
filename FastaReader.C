/***********************************************************************
 FastaReader.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "FastaReader.H"
#include "Exceptions.H"
using namespace std;



BOOM::Regex BOOM::FastaReader::deflineRegex("^\\s*>\\s*(\\S+)(.*)");



BOOM::FastaReader::FastaReader(const BOOM::String &filename,
			       const Alphabet &alphabet)
  : file(filename), alphabet(alphabet)
{
}



BOOM::Vector< pair<BOOM::String,BOOM::String> >*BOOM::FastaReader::readAll()
{
  BOOM::Vector< pair<BOOM::String,BOOM::String> > *v=new 
    BOOM::Vector< pair<BOOM::String,BOOM::String> >;
  BOOM::String defline, sequence;
  while(nextSequence(defline,sequence))
    v->push_back(pair<BOOM::String,BOOM::String>(defline,sequence));
  return v;
}



bool BOOM::FastaReader::nextSequence(BOOM::String &defline,
				     BOOM::String &sequence)
{
   
  if(file.eof()) return false;
  if(cache.length()>0) 
    {
      defline=cache;
      cache="";
    }
  else defline=file.readLine();
  if(file.eof()) return false;

   
  sequence="";
  while(!file.eof())
    {
      BOOM::String line=file.readLine();
      if(line[0]=='>')
	{
	  cache=line;
	  break;
	}
      line.trimWhitespace();
      sequence+=line;
    }
  sequence.toupper();
  maskStrangeChars(sequence);
  return true;
}



void BOOM::FastaReader::maskStrangeChars(BOOM::String &s)
{
  int len=s.length();
  for(int i=0 ; i<len ; ++i)
    if(!alphabet.isDefined(s[i])) {s[i]='N';INTERNAL_ERROR;}
}



void BOOM::FastaReader::parseDefline(const BOOM::String &defline,
				     BOOM::String &id,
				     BOOM::String &remainder)
{
  if(!deflineRegex.match(defline)) 
    throw BOOM::String(
       "BOOM::FastaReader::parseDefline() failed to parse: ")+
      defline;
  id=deflineRegex[1];
  remainder=deflineRegex[2];
}



BOOM::String BOOM::FastaReader::getId(const BOOM::String &defline)
{
  BOOM::String id, remainder;
  parseDefline(defline,id,remainder);
  return id;
}



