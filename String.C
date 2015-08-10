/***********************************************************************
 String.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <sstream>
#include <iostream>
#include "String.H"
#include <string.h>
#include "StrTokenizer.H"
#include "Constants.H"
#include "Hashpjw.H"
using namespace std;
using namespace BOOM;


BOOM::String::String(int n,char c) : string(n,c)
{
  // create string of n c's
}



BOOM::String::String(unsigned u) : string(BOOM::String("")+u)
{
}



BOOM::String::String(unsigned long ul) : string(BOOM::String("")+ul)
{
}



BOOM::String::String(long l) : string(BOOM::String("")+l)
{
}



BOOM::String::String(int i) : string(BOOM::String("")+i)
{
}



BOOM::String::String(float f) : string(BOOM::String("")+f)
{
}



BOOM::String::String(double d) : string(BOOM::String("")+d)
{
}



BOOM::String::String(const char *cp,unsigned length) : string(cp,length)
{
}



BOOM::String::String(const char *p) : string(p?p:"")
{
}



BOOM::String::String(const BOOM::String &s) : string(s)
{
}



BOOM::String::String(const string &s) : string(s)
{
}



BOOM::String::String(char c) : string(BOOM::String("")+c)
{
}



BOOM::String::String() {}


BOOM::String::~String() {}


ostream &operator<<(ostream &os,const BOOM::String &str)
{
  os << str.c_str();
  return os;
}



BOOM::String BOOM::String::operator+(unsigned u) const
{
  stringstream os;
  os << c_str() << u;
  return os.str();
}



int BOOM::String::count(char c)
{
    const char *p=c_str();
    int n=0;
    char x=*p;
    while(x)
    {
        if(x==c) ++n;
        ++p;
        x=*p;
    }
    return n;
}



BOOM::String BOOM::String::operator+(unsigned long ul) const
{
  stringstream os;
  os << c_str() << ul;
  return os.str();
}



BOOM::String BOOM::String::operator+(long l) const
{
  stringstream os;
  os << c_str() << l;
  return os.str();
}



BOOM::String BOOM::String::operator+(int i) const
{
  stringstream os;
  os << c_str() << i;
  return os.str();
}



BOOM::String BOOM::String::operator+(float f) const
{
  stringstream os;
  os << c_str() << f;
  return os.str();
}



BOOM::String BOOM::String::operator+(double d) const
{
  stringstream os;
  os << c_str() << d;
  return os.str();
}



BOOM::String &BOOM::String::tolower()
{
  BOOM::String &self=*this;
  int l=length();
  for(int i=0 ; i<l ; ++i)
    self[i]=::tolower(self[i]);
  return *this;
}



BOOM::String &BOOM::String::toupper()
{
  BOOM::String &self=*this;
  int len=length();
  for(int i=0 ; i<len ; ++i)
    self[i]=::toupper(self[i]);
  return *this;
}



BOOM::String BOOM::String::substitute(const BOOM::String &from,
				  const BOOM::String &to) const
{
  BOOM::String rval;
  const char *pattern=from.c_str();
  int patternLen=from.length();
  const char *ptr=c_str();
  const char *last=ptr+length()-patternLen;
  while(ptr<=last)
    {
      if(localMatch(ptr,pattern,patternLen))
	{
	  ptr+=patternLen;
	  rval+=to;
	}
      else
	{
	  rval+=*ptr;
	  ptr++;
	}
    }
  int extra=patternLen-1;
  //for(int i=0 ; i<extra ; ++i) rval+=*ptr++;
  for(; *ptr ; ++ptr) rval+=*ptr;
  return rval;
}



BOOM::String BOOM::String::substring(int begin,int len) const
{
  return len>=0 ? substr(begin,len) : substr(begin);
}



void BOOM::String::getFields(Vector<String> &into,const char *seps) const
{
  into.clear();
  BOOM::String &self=const_cast<BOOM::String&>(*this);
  BOOM::StrTokenizer tokenizer(self,seps);
  tokenizer.getTokenStrings(into);
}



BOOM::Vector<BOOM::String> *BOOM::String::getFields(const char *seps) const
{
  BOOM::String &self=const_cast<BOOM::String&>(*this);
  BOOM::StrTokenizer tokenizer(self,seps);
  BOOM::Vector<BOOM::String> *fields=tokenizer.getTokenStrings();
  return fields;
}



bool BOOM::String::contains(const BOOM::String &s) const
{
  return find(s.c_str())!=npos;
}



bool BOOM::String::isWordChar(char c)
{
  return (c>='A' && c<='Z') || (c>='a' && c<='z');
}



bool BOOM::String::containsWordChar()
{
  for(const char *ptr=c_str() ; *ptr ; ++ptr)
    {
      char c=*ptr;
      if(isWordChar(c)) return true;
    }
  return false;
}



bool BOOM::String::occursAt(const BOOM::String &substring,int pos) const
{
  return localMatch(substring.c_str(),c_str()+pos,substring.size());
}



bool BOOM::String::localMatch(const char *s1,const char *s2,int len) const
{
  for(int i=0 ; i<len ; ++i)
    if(*s1++!=*s2++) 
      return false;
  return true;
}



bool BOOM::String::stricmp(const BOOM::String &str) const
{
  return strcasecmp(c_str(),str.c_str());
}



void BOOM::String::chop()
{
  int len=length();
  resize(len-1);
}



void BOOM::String::trimWhitespace()
{
  BOOM::String &self=*this;
  const char *begin=c_str();
  while(*begin && isspace(*begin)) 
    ++begin;
  const char *end=begin;
  while(*end) 
    ++end;
  if(end>begin) 
    --end;
  while(end>begin && isspace(*end)) 
    --end;
  int len=end-begin+2;
  char *buf=new char[len];
  char *ptr=buf;
  while(begin<=end) 
    *ptr++=*begin++;
  *ptr='\0';
  self=buf;
  delete [] buf;
}



BOOM::String BOOM::String::operator+(const char *p)const
{
    return BOOM::String((*(string*)this)+p);
}


BOOM::String BOOM::String::operator+(char *p) const
{
  return BOOM::String((*(string*)this)+p);
}


BOOM::String BOOM::String::operator+(const string &s) const
{
  return BOOM::String(*this+s.c_str());
}


BOOM::String BOOM::String::operator+(const BOOM::String &s) const
{
  return BOOM::String(*this+s.c_str());
}


BOOM::String BOOM::String::operator+(char c) const
{
  char str[2];
  str[0]=c;
  str[1]='\0';
  return BOOM::String(*this+(const char*)str);
}



BOOM::String BOOM::String::toHex()
{
  const char *p=c_str();
  int n=length();
  BOOM::String hex;
  for(int i=0 ; i<n ; ++i, ++p)
    {
      unsigned char c=*p;
      hex+="\\x";
      hex+=decimalToHex(unsigned(c));
    }
  return hex;
}


BOOM::String BOOM::String::decimalToHex(unsigned d)
{
  BOOM::String hex;
  if(d==0) return "0";
  while(d)
    {
      int digitValue=d%16;
      d/=16;
      char c;
      if(digitValue<10) c=char(digitValue)+'0';
      else c=char(digitValue-10)+'A';
      hex=BOOM::String("")+c+hex;
    }
  return hex;
}



void BOOM::String::padOrTruncate(int desiredLength)
{
  resize(desiredLength,' ');
}



unsigned BOOM::String::hash() const
{
  return Hashpjw(*this,LARGEST_UNSIGNED_INT);
}



void String::replaceSubstring(int start,int length,const String &other)
{
  replace(start,length,other);
}



int String::findFirst(char c) const
{
  const char *p=c_str();
  int pos=0;
  for(; *p ; ++p, ++pos) if(*p==c) return pos;
  return -1;
}

