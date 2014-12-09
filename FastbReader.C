/****************************************************************
 FastbReader.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "FastbReader.H"
#include "FastaWriter.H"
#include "Constants.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;


Regex FastbReader::discreteRegex(">(\\S+)(.*)");
Regex FastbReader::continuousRegex("%(\\S+)(.*)");
Regex FastbReader::keyValueRegex("/([^=]+)=(\\S+)");


/****************************************************************
                      FastbSequence methods
 ****************************************************************/
FastbSequence::FastbSequence(FastbSeqType type,const String &defline,
			     const String &ID)
  : type(type), defline(defline), ID(ID)
{
  // ctor
}



const String &FastbSequence::getDefline() const
{
  return defline;
}



const String &FastbSequence::getID() const
{
  return ID;
}



FastbSeqType FastbSequence::getType() const
{
  return type;
}



bool FastbSequence::isDiscrete() const
{
  return type==FASTB_DISCRETE;
}



bool FastbSequence::isContinuous() const
{
  return type==FASTB_CONTINUOUS;
}



ostream &BOOM::operator<<(ostream &os,const FastbSequence &seq)
{
  seq.printOn(os);
  return os;
}



/****************************************************************
                    FastbDiscreteSeq methods
 ****************************************************************/
FastbDiscreteSeq::FastbDiscreteSeq(const String &s,const String &defline,
				   const String &ID)
  : FastbSequence(FASTB_DISCRETE,defline,ID), seq(s)
{
  // ctor
}



const String &FastbDiscreteSeq::getSeq() const
{
  return seq;
}



void FastbDiscreteSeq::printOn(ostream &os) const
{
  FastaWriter writer;
  writer.addToFasta(getDefline(),seq,os);
}



/****************************************************************
                   FastbContinuousSeq methods
 ****************************************************************/
FastbContinuousSeq::FastbContinuousSeq(const DoubleVector &s,
				       const String &defline,const String &ID)
  : FastbSequence(FASTB_CONTINUOUS,defline,ID), seq(s)
{
  // ctor
}



const DoubleVector &FastbContinuousSeq::getSeq() const
{
  return seq;
}



void FastbContinuousSeq::printOn(ostream &os) const
{
  os<<getDefline()<<endl;
  int N=seq.size();
  for(int i=0 ; i<N ; ++i) os<<seq[i]<<endl;
}



/****************************************************************
                       FastbReader methods
 ****************************************************************/
FastbReader::FastbReader()
{
  // default ctor
}



FastbReader::FastbReader(const String &filename)
{
  if(!file.open(filename)) throw String("Can't open file: ")+filename;
}



void FastbReader::open(const String &filename)
{
  if(!file.open(filename)) throw String("Can't open file: ")+filename;
}



FastbSequence *FastbReader::nextSequence()
{
  String defline, ID;

  // Read defline
  if(!nextDefline.isEmpty()) {
    defline=nextDefline;
    nextDefline="";
  }
  else {
    bool found=false;
    while(!file.eof()) {
      String line=file.getline();
      if(!line.isEmpty()) {defline=line; found=true; break;}
    }
    if(!found) return NULL;
  }
  FastbSeqType seqType;
  Regex *regex;
  if(discreteRegex.match(defline)) {
    ID=discreteRegex[1]; 
    seqType=FASTB_DISCRETE;}
  else if(continuousRegex.match(defline)) {
    ID=continuousRegex[1];
    seqType=FASTB_CONTINUOUS;}
  else throw String("Bad defline in FASTB file: ")+defline;

  // Read sequence data
  String discreteSeq;
  DoubleVector continuousSeq;
  while(!file.eof()) {
    String line=file.getline();
    line.trimWhitespace();
    if(file.eof() && line.isEmpty()) break;
    if(discreteRegex.match(line) || continuousRegex.match(line)) 
      {nextDefline=line; break;}
    else switch(seqType)
      {
      case FASTB_DISCRETE:
	discreteSeq+=line;
	break;
      case FASTB_CONTINUOUS: 
	{
	  const double d=line.asDouble();
	  //continuousSeq.push_back(isFinite(d) ? d : 0);

	  continuousSeq.push_back(d);//### 
	  if(!isFinite(d)) //### represents "missing data"
	    cerr<<"warning: non-finite value encountered: "<<d<<endl;//###
	}
	break;
      }
  }
  if(seqType==FASTB_DISCRETE) 
    return new FastbDiscreteSeq(discreteSeq,defline,ID);
  else 
    return new FastbContinuousSeq(continuousSeq,defline,ID);
}



bool FastbReader::eof()
{
  return file.eof();
}



StringPairVector *FastbReader::parseDefline(const String &defline,String &ID)
{
  String rest;
  FastbSeqType seqType;
  if(discreteRegex.match(defline)) {
    seqType=FASTB_DISCRETE;
    ID=discreteRegex[1];
    rest=discreteRegex[2];
  }
  else if(continuousRegex.match(defline)) {
    seqType=FASTB_CONTINUOUS;
    ID=continuousRegex[1];
    rest=continuousRegex[2];
  }
  else throw String("Bad FASTC defline: ")+defline;
  StringPairVector *fields=new StringPairVector;
  rest.trimWhitespace();
  Vector<String> *strFields=rest.getFields(" \t");
  Vector<String>::iterator cur=strFields->begin(), end=strFields->end();
  for(; cur!=end ; ++cur) {
    String field=*cur;
    if(!keyValueRegex.match(field)) 
      throw String("Can't parse FASTC defline regex: ")+field;
    String key=keyValueRegex[1], value=keyValueRegex[2];
    fields->push_back(pair<String,String>(key,value));
  }
  delete strFields;
  return fields;
}








