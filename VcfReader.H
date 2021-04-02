/****************************************************************
 VcfReader.H
 Copyright (C)2017 William H. Majoros (martiandna@gmail.com).
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
#include "Comparator.H"
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
  String &getChr();
  int getPos() const;
  int getBegin() const { return pos; }
  int getEnd() const;
  void printOn(ostream &) const;
  bool containsNonstandardAlleles() const;
  void setText(const String &);
  const String &getText() const;
  bool isIndel() const;
private:
  String id, chr;
  int pos;
  Vector<String> alleles;
  String text;
};
ostream &operator<<(ostream &,const Variant &);

class VariantComparator : public Comparator<Variant> {
public:
  bool equal(Variant &a,Variant &b) 
  { return a.getPos()==b.getPos(); }
  bool greater(Variant &a,Variant &b) 
  { return a.getPos()>b.getPos(); }
  bool less(Variant &a,Variant &b) 
  { return a.getPos()<b.getPos(); }
};

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
  String getText() const;
  bool anyAltAlleles() const;
private:
  Vector<int> alleles;
};
ostream &operator<<(ostream &,const Genotype &);

/****************************************************************
                         VariantAndGenotypes
 ****************************************************************/
struct VariantAndGenotypes {
  VariantAndGenotypes() {}
  VariantAndGenotypes(Variant &v,Vector<Genotype> &g) 
    : variant(v), genotypes(g) {}
  Variant variant;
  Vector<Genotype> genotypes;
};

class VariantAndGenotypesComparator : public Comparator<VariantAndGenotypes>{
public:
  bool equal(VariantAndGenotypes &a,VariantAndGenotypes &b) 
  { return a.variant.getPos()==b.variant.getPos(); }
  bool greater(VariantAndGenotypes &a,VariantAndGenotypes &b) 
  { return a.variant.getPos()>b.variant.getPos(); }
  bool less(VariantAndGenotypes &a,VariantAndGenotypes &b) 
  { return a.variant.getPos()<b.variant.getPos(); }
};

/****************************************************************
                             VcfReader
 ****************************************************************/
class VcfReader {
public:
  VcfReader(const String &filename); // can be gzipped (*.gz)
  virtual ~VcfReader();
  const Vector<String> &getSampleIDs() const;
  const Vector<String> &getHeaderLines() const;
  const String &getChromLine() const;
  bool nextVariant(Variant &,Vector<Genotype> &);// calls advance()
  bool nextVariant(VariantAndGenotypes &);// calls advance()
  bool currentVariant(Variant &,Vector<Genotype> &);// doesn't advance()
  bool currentVariant(VariantAndGenotypes &);// doesn't advance()
  void advance(); // loads next variant into buffer
  void close();
  static void getVariableSites(const String &filename,Vector<Variant> &,
			       int &totalSites,int &numVarSites);
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
  void parseChromLine();
  bool parseVariant();
  void parseGenotype(const String &,Genotype &into);
};

}

#endif

