/***********************************************************************
 Exceptions.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Exceptions_H
#define INCL_BOOM_Exceptions_H

namespace BOOM {
#define INTERNAL_ERROR {cout<<"INTERNAL ERROR in "<<__FILE__<<" at line "<<__LINE__<<endl;throw "exiting";}
#define TRACE {cout<<"trace "<<__FILE__<<" "<<__LINE__<<endl;}
#define CHECKPOINT {cout<<__FILE__<<" "<<__LINE__;}
}

#include "String.H"
using namespace std;

namespace BOOM {

#define Endl endl

class RootException
{
public:
  RootException(const char *);
  RootException(const BOOM::String &);
  const BOOM::String &getMessage() const;
  void setMessage(const String &m);
protected:
  String message;
};

class FileErrorException : public RootException
{
public:
  FileErrorException(BOOM::String filename,BOOM::String msg);
};

class ArrayIndexException : public RootException
{
public:
  ArrayIndexException(long index,const BOOM::String &msg);
private:
  void installMessage(long index,const BOOM::String &);
};
}

#endif
