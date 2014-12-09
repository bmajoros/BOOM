/****************************************************************
 CFasta.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "CFasta.H"
using namespace std;
using namespace BOOM;

CFasta::CFasta(const String &filename)
  : f(filename), deflineRegex(">(\\S+).*"), filename(filename)
{
  // ctor
}



CFasta::~CFasta()
{
  if(f.isOpen()) f.close();
}



void CFasta::close()
{
  f.close();
}



bool CFasta::getNextSequence(String &defline,String &id,
			     Vector<double> &values)
{
  // First, skip blank lines
  String line;
  if(!nextDefline.isEmpty()) { line=nextDefline; nextDefline=""; }
  else while(!f.eof()) {
    line=f.getline();
    if(!line.isEmpty()) break;
  }
  if(line.isEmpty()) return false;

  // Parse defline
  if(!deflineRegex.match(line)) 
    throw String("Can't parse defline in file ")+filename+" : "+line;
  id=deflineRegex[1];

  // Read sequence values
  while(!f.eof()) {
    line=f.getline();
    if(deflineRegex.match(line)) {
      nextDefline=line;
      return true;
    }
    BOOM::Vector<BOOM::String> &fields=*line.getFields();
    BOOM::Vector<BOOM::String>::iterator cur=fields.begin(), end=fields.end();
    for(; cur!=end ; ++cur) {
      double x=(*cur).asDouble();
      values.push_back(x);
    }
    delete &fields;
  }

  return true;
}



bool CFasta::eof()
{
  return f.eof();
}





