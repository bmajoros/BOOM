/***********************************************************************
 HugeFastaReader.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "HugeFastaReader.H"
#include <iostream>
#include <string.h>
using namespace std;


BOOM::HugeFastaReader::HugeFastaReader(const BOOM::String &filename,
					 int chunkSize,
					 int overlapSize)
  : file(filename), chunkSize(chunkSize),
    overlapSize(overlapSize), buffer(new char[chunkSize+1])
{
  if(overlapSize<0 || overlapSize>=chunkSize)
    throw "Bad overlap size in BOOM::HugeFastaReader";

  remainderSize=chunkSize-overlapSize;
  overlap=buffer+remainderSize;
  remainder=buffer+overlapSize;
  end=buffer+chunkSize;
  defline=file.getline();
  loadFirstChunk();
}



BOOM::HugeFastaReader::~HugeFastaReader()
{
  delete [] buffer;
}



const char *BOOM::HugeFastaReader::getBuffer()
{
  return buffer;
}



void BOOM::HugeFastaReader::loadFirstChunk()
{
  int bytesRead=file.read(chunkSize,buffer);
  buffer[bytesRead]='\0';
  filterSpaces(buffer);
}



bool BOOM::HugeFastaReader::loadNextChunk()
{
  // First, move overlap region to beginning of buffer
  memmove(buffer,overlap,overlapSize);
  
  // Now load remainder of chunk into the rest of the buffer
  int bytesRead=file.read(remainderSize,remainder);
  remainder[bytesRead]='\0';
  filterSpaces(remainder);
  return !file.eof();
}



bool BOOM::HugeFastaReader::isMore()
{
  return !file.eof();
}



void BOOM::HugeFastaReader::filterSpaces(char *buf)
{
  // Compact the sequence to remove any spaces
  char *src=buf, *dest=buf;
  for( ; *src ; ++src)
    {
      char c=*src;
      if(isspace(c)) continue;
      if(src>dest) *dest=c;
      ++dest;
    }

  // Now read in any additional bases to fill up the rest of the buffer
  while(dest!=end && !file.eof())
    {
      if(!file.read(1,dest)) break;
      if(!isspace(*dest)) ++dest;
    }
  *dest='\0';
}



const BOOM::String &BOOM::HugeFastaReader::getDefline()
{
  return defline;
}


