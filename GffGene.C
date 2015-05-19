/****************************************************************
 GffGene.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GffGene.H"
using namespace std;
using namespace BOOM;

GffGene::GffGene(const String &id)
  : id(id)
{
  // ctor
}



GffGene::~GffGene()
{
  for(Vector<GffTranscript*>::iterator cur=transcripts.begin(), 
	end=transcripts.end() ; cur!=end ; ++cur)
    delete *cur;
}



void GffGene::addTranscript(GffTranscript *transcript)
{
  transcripts.push_back(transcript);
}



int GffGene::numTranscripts() const
{
  return transcripts.size();
}



GffTranscript &GffGene::getIthTranscript(int i)
{
  return *transcripts[i];
}



int GffGene::getBegin() const
{
  return begin;
}



int GffGene::getEnd() const
{
  return end;
}



const String &GffGene::getID() const
{
  return id;
}



