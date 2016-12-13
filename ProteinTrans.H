/***********************************************************************
 ProteinTrans.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_ProteinTrans_H
#define INCL_BOOM_ProteinTrans_H
#include <string.h>
#include "String.H"
#include "Map.H"
#include "Sequence.H"
#include "Alphabet.H"
using namespace std;

namespace BOOM {
class ProteinTrans
{
public:
  static BOOM::String reverseComplement(const BOOM::String &);
  static BOOM::Sequence *reverseComplement(const BOOM::Sequence &);
  static BOOM::String translate(const BOOM::String &transcript,int frame=0);
  static Sequence *translate(const Sequence &,Alphabet &dnaAlphabet,
			     Alphabet &proteinAlphabet,int frame=0);
  static void translate(const Sequence &source,Sequence &dest,
			int frame=0);
  static Sequence *translate(const Sequence &,int frame=0);
  static void translate(const Sequence &source,Alphabet &dnaAlphabet,
			Alphabet &proteinAlphabet,Sequence &dest,
			int frame=0);
  static char mapCodon(const char *codon);
private:
  static ProteinTrans initializer;
  struct mapstrcmp
  {
    bool operator()(const char *p,const char *q) const
      {	return strcmp(p,q)<0; }
  };
  static BOOM::Map<const char *,char,mapstrcmp> codonMap;
  ProteinTrans();
  static char complement(char);
  void setupCodonMap();
};
}

#endif
