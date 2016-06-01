/****************************************************************
 VcfReader.H
 Copyright (C)2016 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_VcfReader_H
#define INCL_VcfReader_H
#include <iostream>
#include "File.H"
#include "String.H"
using namespace std;
using namespace BOOM;

namespace BOOM {

/****************************************************************
                            Variant
 ****************************************************************/
class Variant {
public:
  Variant() {};
  Variant(const String &id,const String &chr,int pos);
  void addAllele(const String &);
  int numAlleles() const;
  const String &getAllele(int) const; // 0=ref
  const String &getID() const;
  const String &getChr() const;
  int getPos() const;
private:
  String id, chr;
  int pos;
  Vector<String> alleles;
};

/****************************************************************
                             Genotype
 ****************************************************************/
class Genotype {
public:
  Genotype() {}
  void addAllele(int);
  int numAlleles() const;
  int getAllele(int);
private:
  Vector<int> alleles;
};

/****************************************************************
                             VcfReader
 ****************************************************************/
class VcfReader {
public:
  VcfReader(const String &filename);
  const Vector<String> &getSampleIDs() const;
  bool nextVariant(Variant &,Vector<Genotype> &);
private:
  Vector<String> sampleIDs;
  File file;
  void advance();
};

}

#endif

