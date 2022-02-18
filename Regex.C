/***********************************************************************
 Regex.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Regex.H"
#include "Exceptions.H"
#include "Vector.H"
using namespace std;


BOOM::Map<String,Regex*> Regex::precompiled;



BOOM::Regex::Regex(const BOOM::String &regex)
  : regex(preprocess(regex)), substrate(NULL)
{
  reg.start=NULL;
  reg.end=NULL;
  compile();
}



BOOM::Regex::~Regex()
{
  regfree(&patbuf);
  if(reg.start) free(reg.start);
  if(reg.end) free(reg.end);
}



BOOM::String BOOM::Regex::operator[](int rNum)
{
  int fr=reg.start[rNum], to=reg.end[rNum];
  int length=to-fr;
  if(!substrate) throw "No substrate in BOOM::Regex::operator[]";
  return substrate->substring(fr,length);
}



BOOM::String BOOM::Regex::getEntireMatch() 
{
  return (*this)[0];
}



BOOM::String BOOM::Regex::preprocess(const BOOM::String &express)
{
  BOOM::String rval;
  const char *ptr=express.c_str();
  while(*ptr)
    {
      if(*ptr=='\\')
	{
	  ++ptr;
	  switch(*ptr)
	    {
	    case 'P':  rval+="[^[:print:]]"; break;
	    case 'U':  rval+="[^[:upper:]]"; break;
	    case 'a':  rval+="[[:alpha:]]"; break;
	    case 's':  rval+="[[:blank:][:space:]]"; break;
	    case 'S':  rval+="[^[:blank:][:space:]]"; break;
	    case 'G':  rval+="[^[:graph:]]"; break;
	    case 'L':  rval+="[^[:lower:]]"; break;
	    case 'u':  rval+="[[:upper:]]"; break;
	    case 'c':  rval+="[[:cntrl:]]"; break;
	    case 'D':  rval+="[^[:digit:]]"; break;
	    case 'x':  rval+="[[:xdigit:]]"; break;
	    case 'w':  rval+="[[:alnum:]]"; break;
	    case 'l':  rval+="[[:lower:]]"; break;
	    case 'C':  rval+="[^[:cntrl:]]"; break;
	    case '\0': rval+='\\'; continue;
	    case 'd':  rval+="[[:digit:]]"; break;
	    case 'X':  rval+="[^[:xdigit:]]"; break;
	    case 'p':  rval+="[[:print:]]"; break;
	    case 'W':  rval+="[^[:alnum:]]"; break;
	    case 'A':  rval+="[^[:alpha:]]"; break;
	    case 'g':  rval+="[[:graph:]]"; break;
	    case '[':  rval+="\\["; break;
	    case ']':  rval+="\\]"; break;
	    case '(':  rval+="\\("; break;
	    case ')':  rval+="\\)"; break;
	    case '{':  rval+="\\{"; break;
	    case '}':  rval+="\\}"; break;
	    default: rval+=*ptr; break;
	    }
	  ++ptr;
	}
      else
	{
	  rval+=*ptr;
	  ++ptr;
	}
    }
  return rval;
}



BOOM::String BOOM::Regex::substitute(const BOOM::String &regex,
				 const BOOM::String &replacement,
				 const BOOM::String &substrate)
{
  BOOM::Regex r(regex);
  return r.substitute(substrate,replacement);
}



BOOM::String BOOM::Regex::substitute(const BOOM::String &s,
				 const BOOM::String &replacement)
{
  BOOM::String rval;
  int len=s.length(), here=0;
  while(here<len)
    {
      int matchIndex=
	re_search(&patbuf,s.c_str(),s.size(),here,s.size(),&reg);
      if(matchIndex<0) break;
      rval+=s.substring(here,matchIndex-here);
      rval+=replacement;
      int matchLen=reg.end[0]-reg.start[0];
      if(matchLen==0) ++matchLen;
      here=matchIndex+matchLen;
    }
  if(here<len)
    rval+=s.substring(here,len-here);
  return rval;
}



BOOM::Vector<BOOM::String*> *BOOM::Regex::split(const BOOM::String &regex,
					  const BOOM::String &substrate)
{
  BOOM::Regex r(regex);
  return r.split(substrate);
}



BOOM::Vector<BOOM::String*> *BOOM::Regex::split(const BOOM::String 
						  &substrate)
{
  BOOM::Vector<BOOM::String*> *fields=new BOOM::Vector<BOOM::String*>;

  int len=substrate.length(), here=0;
  while(here<len)
    {
      int nextMatchIndex=
	re_search(&patbuf,substrate.c_str(),substrate.size(),here,
		  substrate.size(),&reg);
      if(nextMatchIndex<0) break;
      if(nextMatchIndex>here)
	fields->push_back(
	  new BOOM::String(substrate.substring(here,nextMatchIndex-here)));
      int matchLen=
	reg.end[0]-reg.start[0];
      if(matchLen==0) 
	++matchLen;
      here=nextMatchIndex+matchLen;
    }
  if(here<len)
    fields->push_back(
      new BOOM::String(substrate.substring(here,len-here)));

  return fields;
}



bool BOOM::Regex::match(const BOOM::String &regex,
			const BOOM::String &substrate)
{
  /*
  BOOM::Regex r(regex);
  return r.match(substrate);
  */
  Regex *r;
  if(precompiled.isDefined(regex)) r=precompiled[regex];
  else {
    r=precompiled[regex]=new BOOM::Regex(regex);
  }
  return r->match(substrate);
}



bool BOOM::Regex::match(const BOOM::String &s)
{
  int matchLength=re_match(&patbuf,s.c_str(),s.size(),0,&reg);
  if(matchLength<0)
    {
      substrate=NULL;
      return false;
    }
  substrate=&s;
  return true;
}



bool BOOM::Regex::search(const BOOM::String &regex,
			 const BOOM::String &substrate)
{
  /*
  BOOM::Regex r(regex);
  return r.search(substrate);
  */
  Regex *r;
  if(precompiled.isDefined(regex)) r=precompiled[regex];
  else {
    r=precompiled[regex]=new BOOM::Regex(regex);
  }
  return r->search(substrate);
}



bool BOOM::Regex::search(const BOOM::String &s)
{
  int matchIndex=re_search(&patbuf,s.c_str(),s.size(),0,s.size(),&reg);
  if(matchIndex<0)
    {
      substrate=NULL;
      return false;
    }
  substrate=&s;
  return true;
}



int BOOM::Regex::getNumSubexpressions() const
{
  return numOfSubexpress;
}



void BOOM::Regex::compile()
{
  patbuf.allocated=0;
  patbuf.fastmap=0;
  patbuf.buffer=NULL;
  patbuf.translate=NULL;
  patbuf.syntax=re_syntax_options=
    RE_NO_BK_BRACES  | 
    RE_CHAR_CLASSES |
    RE_NO_BK_PARENS | 
    //RE_INTERVALS |
    RE_NO_BK_VBAR | 
    RE_SYNTAX_EMACS;
  const char *errMsg=re_compile_pattern(regex.c_str(),regex.size(),&patbuf);
  if(errMsg!=NULL) throw BOOM::String(errMsg);
  numOfSubexpress=patbuf.re_nsub;
}



void BOOM::Regex::getMatchIndices(int regNum,int &start,int &end)
{
  start=reg.start[regNum];
  end=reg.end[regNum];
}
