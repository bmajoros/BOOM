/****************************************************************
 GCF.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_GCF_H
#define INCL_GCF_H
#include <iostream>
#include "Vector.H"
#include "Array1D.H"
using namespace std;
using namespace BOOM;

enum HAPLOID_GENOTYPE {
  HG_REF=0,
  HG_ALT=1,
  HG_NONE=-1
};

struct GcfVariant { // biallelic only!
  String id;
  String chr;
  int pos;
  String ref, alt;
  GcfVariant(const String &id,const String &chr,int pos,const String &ref,
	     const String &alt) : chr(chr), pos(pos), ref(ref), alt(alt) {}
};

struct GcfIndividual {
  String id;
  Array1D<int> chrom[2]; // diploid only!
  GcfIndividual(const String &id,int n) : id(id) { chrom[0].resize(n); chrom[1].resize(n); }
};

/****************************************************************
 Represents a GCF file (which is a transposed VCF file, so that
 each line represents an individual and each column represents a
 genetic variant).
 ****************************************************************/

class GCF {
public:
  GCF(const String &filename);
  int numVariants() const;
  int numIndividuals() const;
  const GcfVariant &getIthVariant(int i) const;
  const GcfIndividual &getIthIndividual(int i) const;
protected:
  Vector<GcfIndividual> individuals;
  Vector<GcfVariant> variants;
  void parse(const String &filename);
};

#endif

