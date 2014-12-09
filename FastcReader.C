/****************************************************************
 FastcReader.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "FastcReader.H"
using namespace std;
using namespace BOOM;


Regex FastcReader::deflineRegex("%(\\S+)(.*)");
Regex FastcReader::keyValueRegex("/([^=]+)=(\\S+)");


FastcReader::FastcReader()
{
  // default ctor
}



FastcReader::FastcReader(const String &filename)
  : file(filename)
{
  // ctor
}



void FastcReader::open(const String &filename)
{
  file.open(filename);
}



bool FastcReader::nextSequence(String &defline,String &ID,
			       BOOM::Vector<double> &data)
{
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
    if(!found) return false;
  }
  if(!deflineRegex.match(defline)) 
    throw String("Bad defline in FASTC file: ")+defline;
  ID=deflineRegex[1];

  // Read sequence data
  while(!file.eof()) {
    String line=file.getline();
    if(deflineRegex.match(line)) {nextDefline=line; break;}
    else data.push_back(line.asDouble());
  }
  return true;
}



bool FastcReader::eof()
{
  return file.eof();
}



StringPairVector *FastcReader::parseDefline(const String &defline,
							 String &ID)
{
  if(!deflineRegex.match(defline)) 
    throw String("Bad FASTC defline: ")+defline;
  StringPairVector *fields=new StringPairVector;
  ID=deflineRegex[1];
  String rest=deflineRegex[2];
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




