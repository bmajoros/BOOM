/***********************************************************************
 MemoryProfiler.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
using namespace std;
#include "MemoryProfiler.H"
#include <iostream>
#include "unistd.h"
#include "File.H"
#include "Vector.H"
using namespace BOOM;


void MemoryProfiler::report(const BOOM::String &label,ostream &os)
{
#ifdef __linux    
  int pid=getpid();
  BOOM::String filename=BOOM::String("/proc/")+pid+BOOM::String("/status");
  BOOM::File file(filename,"r");
  long size=-1;
  BOOM::String units;
  while(!file.eof())
    {
      BOOM::String line=file.getline();
      if(file.eof()) break;
      BOOM::Vector<BOOM::String> &fields=*line.getFields();
      if(fields.size()>0 && fields[0]=="VmSize:")
	{
	  size=fields[1].asLong();
	  units=fields[2];
	  delete &fields;
	  break;
	}
      delete &fields;
    }
  os<<label<<" ";
  if(units=="kB" && size>=1000)
    os<<(size/1000)<<" Mb"<<endl;  
  else 
    os<<size<<" "<<units<<endl;
#endif
}


