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
//BOOM::Regex BOOM::FastaReader::attrRegex("/(\\S+)=(\\S+)");



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



static FastaReader::load(const String &filename,String &defline,String &sequence,
			 const Alphabet &alphabet)
{
  FastaReader reader(filename,alphabet);
  reader.nextSequence(defline,sequence);
}



void FastaReader::parseAttributes(const String &remainder,
				  Map<String,String> &attr)
{
  Vector<String> fields;
  remainder.getFields(fields," \t");
  const int N=fields.size();
  for(int i=0 ; i<N ; ++i) {
    /*if(attrRegex.search(fields[i])) {
      cout<<attrRegex[1]<<" => "<<attrRegex[2]<<endl;
      attr[attrRegex[1]]=attrRegex[2];
      }*/
    /*if(attrRegex.search(fields[i]))
      cout<<attrRegex[1]<<endl;
    else cout<<"not found: "<<fields[i]<<endl;
    */
    const String sField=fields[i];
    const int fieldLen=sField.length();
    const char *field=sField.c_str();
    const char *p=field;
    if(*p=='/') {
      while(*p && *p!='=') ++p;
      if(*p=='=') {
	int keyLen=p-field-1;
	String key(field+1,keyLen);
	String value(p+1,fieldLen-keyLen-2);
	attr[key]=value;
      }
    }
  }
}




