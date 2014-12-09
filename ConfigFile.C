/***********************************************************************
 ConfigFile.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <stdlib.h>
#include <fstream>
#include "File.H"
#include "ConfigFile.H"
using namespace std;



BOOM::ConfigFile::ConfigFile(BOOM::String filename)
{
  load(filename);
}



BOOM::ConfigFile::ConfigFile()
{
}



bool BOOM::ConfigFile::isDefined(BOOM::String key)
{
  return dict.isDefined(key);
}



BOOM::String BOOM::ConfigFile::lookup(BOOM::String attr)
{
  return dict[attr];
}



BOOM::String BOOM::ConfigFile::lookupOrDie(BOOM::String attr)
{
  return charLookupOrDie(attr);
}



const char *BOOM::ConfigFile::charLookupOrDie(BOOM::String attr)
{
  if(dict.find(attr)==dict.end()) 
    throw attr+" is not defined in the configuration file";
  return dict[attr].c_str();
}



double BOOM::ConfigFile::getDoubleOrDie(BOOM::String attr)
{
  return atof(charLookupOrDie(attr));
}



float BOOM::ConfigFile::getFloatOrDie(BOOM::String attr)
{
  return (float) atof(charLookupOrDie(attr));
}



int BOOM::ConfigFile::getIntOrDie(BOOM::String attr)
{
  return atoi(charLookupOrDie(attr));
}



bool BOOM::ConfigFile::getBoolOrDie(BOOM::String attr)
{
  BOOM::String b=charLookupOrDie(attr);
  if(b=="true" || b=="t" || b=="yes" || b=="y") return true;
  if(b=="false" || b=="f" || b=="no" || b=="n") return false;
  throw attr+" must be true or false in config file";
}



long BOOM::ConfigFile::getLongOrDie(BOOM::String attr)
{
  return atol(charLookupOrDie(attr));
}



void BOOM::ConfigFile::enter(BOOM::String key,BOOM::String value)
{
  dict[key]=value;
}



void BOOM::ConfigFile::load(BOOM::String fname)
{
  BOOM::File file(fname);
  if(!file.isOpen())
    throw BOOM::String("Error: Can't open file ")+fname+
      " in BOOM::ConfigFile::load()";
  while(!file.eof())
    {
      BOOM::String nextline=file.getline();
      
      if(file.eof()) break;
      const char *p=nextline.c_str();
      BOOM::StrTokenizer parser(p,"=\n\t \r");
      processLine(parser);
    }
}



void BOOM::ConfigFile::processLine(BOOM::StrTokenizer &parser)
{
  if(!parser.hasMoreTokens()) return;  
  BOOM::String key=parser.nextToken();
  if('#'==key[0]) return; // comment
  if(!parser.hasMoreTokens())  
    throw BOOM::String("Syntax error in configuration file: "+key);
  const char *value=parser.nextToken();
  dict[key]=value; 
}



BOOM::Map<BOOM::String,BOOM::String> &BOOM::ConfigFile::peek()
{
  return dict;
}


