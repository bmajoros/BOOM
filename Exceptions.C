/***********************************************************************
 Exceptions.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Exceptions.H"
#include "String.H"
using namespace std;
using namespace BOOM;


ArrayIndexException::ArrayIndexException(long index,
					       const BOOM::String &msg)
  : RootException("")
{
   

  installMessage(index,msg);
}



RootException::RootException(const BOOM::String &s)
  : message(s)
{
   
}



RootException::RootException(const char *cp)
  : message(cp)
{
   
}



void RootException::setMessage(const String &m) 
{
  message=m;
}



const BOOM::String &RootException::getMessage() const
{
  return message;
}



void ArrayIndexException::installMessage(long index,const BOOM::String &msg)
{
  message=BOOM::String("Invalid index (")+index+") "+msg;
}



FileErrorException::FileErrorException(BOOM::String filename,BOOM::String msg)
  : RootException(filename+" "+msg) 
{
}


