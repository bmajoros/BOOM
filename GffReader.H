/***********************************************************************
 GffReader.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_GffReader_H
#define INCL_BOOM_GffReader_H
#include "Vector.H"
#include "String.H"
#include "Set.H"
#include "Regex.H"
#include "GffTranscript.H"
#include "GffGene.H"
#include "GffFeature.H"
#include "File.H"
#include "Comparator.H"
using namespace std;

namespace BOOM {

typedef Vector<GffTranscript*> TranscriptList;

class GffReader {
public:
  typedef Vector<GffTranscript*> TranscriptList;
  GffReader(const String &filename);
  void close();
  GffFeature *nextFeature();
  Vector<GffFeature*> *loadFeatures(); // sorted by begin coord
  static void addNoncodingFeatures(Vector<GffFeature*> &,int substrateLength,
				   const String &substrate,
				   const String &source,
				   int maxFeatureLength=20000);
  static void filterBySource(Vector<GffFeature*> &,const String &source);
  TranscriptList *loadTranscripts();
  Vector<GffGene> *loadGenes();
  static Vector<GffGene> *loadGenes(const String &filename);
  static Map<String,Vector<GffGene> > *genesByChrom(const String &filename);
  static TranscriptList *loadTranscripts(const String &filename);
  Map<String,TranscriptList*> *loadByContig(); // organized by substrate
  Map<String,Vector<GffFeature*> > *featuresBySubstrate();
private:
  Set<String> exonTypes;
  Regex commentPattern, transgrpRegex, geneIdRegex;
  File file;
};
}

#endif
