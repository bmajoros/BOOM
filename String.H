/***********************************************************************
 String.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_String_H
#define INCL_BOOM_String_H
#include <stdlib.h>
#include <iostream>
#include <string>
#undef toupper
#undef tolower
using namespace std;

namespace BOOM {

template<class T>
class Vector;

class String : public string {
public:
  String(int n,char c); // create string of n c's
  String(unsigned);
  String(unsigned long);
  String(long);
  String(int);
  String(float);
  String(double);
  String(const char *,unsigned len);
  String(const char *);
  String(const BOOM::String &);
  String(const string &);
  String(char);
  String();
  virtual ~String();
  int getLength() { return int(length()); }
  BOOM::String operator+(unsigned) const;
  BOOM::String operator+(unsigned long) const;
  BOOM::String operator+(long) const;
  BOOM::String operator+(int) const;
  BOOM::String operator+(float) const;
  BOOM::String operator+(double) const;
  BOOM::String operator+(char *p) const;
  BOOM::String operator+(const char *p) const;
  BOOM::String operator+(const string &s) const;
  BOOM::String operator+(const BOOM::String &s) const;
  BOOM::String operator+(char c) const;
  BOOM::String &operator+=(const String &s);
  operator float() const {return asFloat();}
  operator double() const {return asDouble();}
  operator int() const {return asInt();}
  operator long() const {return asLong();}
  operator unsigned() const {return asUnsigned();}
  BOOM::String &tolower(); // lowercases self
  BOOM::String &toupper(); // uppercases self
  BOOM::String substitute(const BOOM::String &changeThis,  // CONST!!!!
			  const BOOM::String &toThis) const;
  BOOM::String substring(int start,int length=-1) const;
  void replaceSubstring(int start,int length,const String &); // may be unequal lengths
  BOOM::Vector<BOOM::String> *getFields(const char *separators=" \t\n\r") 
    const;
  void getFields(Vector<String> &into,const char *separators=" \t\n\r") const;
  int count(char);
  bool contains(const BOOM::String &searchString) const;
  bool occursAt(const BOOM::String &substring,int pos) const;
  bool containsWordChar();
  int findFirst(char) const; // -1 if not found, otherwise position
  inline bool isWordChar(char c);
  bool stricmp(const BOOM::String &) const;
  char lastChar() const { return (*this)[length()-1]; }
  double asDouble() const { return atof(c_str()); }
  float asFloat() const { return atof(c_str()); }
  int asInt() const { return atoi(c_str()); }
  long asLong() const { return atol(c_str()); }
  unsigned asUnsigned() const { return asLong(); }
  void chop();
  void trimWhitespace(); // only at ends
  void removeWhitespace();
  inline bool isEmpty() {return empty();}
  void getline(istream &is) {std::getline(is,*this);}
  BOOM::String toHex(); // for writing strings into source code safely
  void padOrTruncate(int desiredLength);
  unsigned hash() const;
private:
  bool localMatch(const char *,const char *,int) const;
  BOOM::String decimalToHex(unsigned);
};
}

ostream &operator<<(ostream &,const BOOM::String &);

//This is bad!!! Do not uncomment the following line!!!
//inline istream &operator>>(istream &is,BOOM::String &s) {return std::getline(is,s);}


#endif
