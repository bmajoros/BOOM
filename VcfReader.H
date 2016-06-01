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
#include "Vector.H"
#include "Regex.H"
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
  void printOn(ostream &) const;
  bool containsNonstandardAlleles() const;
  void setText(const String &);
  const String &getText() const;
private:
  String id, chr;
  int pos;
  Vector<String> alleles;
  String text;
};
ostream &operator<<(ostream &,const Variant &);

/****************************************************************
                             Genotype
 ****************************************************************/
class Genotype {
public:
  Genotype() {}
  void addAllele(int);
  int numAlleles() const;
  int getAllele(int) const;
  int operator[](int i) const { return getAllele(i); }
  void printOn(ostream &) const;
private:
  Vector<int> alleles;
};
ostream &operator<<(ostream &,const Genotype &);

/****************************************************************
                             VcfReader
 ****************************************************************/
class VcfReader {
public:
  VcfReader(const String &filename);
  virtual ~VcfReader();
  const Vector<String> &getSampleIDs() const;
  const Vector<String> &getHeaderLines() const;
  const String &getChromLine() const;
  bool nextVariant(Variant &,Vector<Genotype> &);
  void close();
private:
  Vector<String> sampleIDs;
  Vector<String> headerLines;
  String chromLine;
  Regex gzRegex; // .gz
  Regex CNregex; // <CN14>
  File *file;
  Vector<String> fields;
  Variant variant;
  Vector<Genotype> genotypes;
  void advance();
  void parseChromLine();
  bool parseVariant();
  void parseGenotype(const String &,Genotype &into);
};

}

#endif

