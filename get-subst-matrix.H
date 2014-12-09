/*
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 */
#ifndef INCL_get_subst_matrix_H
#define INCL_get_subst_matrix_H
#include <iostream>
#include "CommandLine.H"
#include "FastaReader.H"
#include "TriangMatrix.H"
#include "Array1D.H"
#include "ConfigFile.H"
#include "ProteinTrans.H"
#include "Random.H"
#include "Exceptions.H"
#include "AminoAlphabet.H"
#include "DnaAlphabet.H"
#include "Time.H"
#include "SubstitutionMatrix.H"
#include "Needleman.H"
#include "exonomy/GraphBuilder.H"
#include "exonomy/GraphReducer.H"
#include "exonomy/PseudoStopDetector.H"

const int LOCAL_OPTIMALITY_DISTANCE=30;
const int NUM_ORF_PAIRS=300;

typedef Vector<ParseEdge*> ORF;
typedef Vector<ParseVertex*> VertexList;

class Application
{
public:
  Application();
  int main(int argc,char *argv[]);
private:
  bool shouldQuantize;
  TriangMatrix<double> *matrix;
  Array1D<double> *marginals;
  double gapOpenPenalty, gapExtensionPenalty;
  double randomOpenPenalty, randomExtensionPenalty;
  Vector<SignalDetector*> signalDetectors;
  GraphBuilder *graphBuilder;
  GraphReducer reducer;
  String infile1, infile2;

  void processOrthologues(const String &proteinFile1,
			  const String &proteinFile2,
			  SubstitutionMatrix &blosumMatrix,
			  double blosumGapOpen,double blosumGapExtend);
  void processDNA(const String &dnaFile1,const String &dnaFile2,
		  SubstitutionMatrix &blosumMatrix,
		  double blosumGapOpen,double blosumGapExtend);
  void produceOutput();
  Vector<Sequence*> *loadSequences(const String &filename,
				       Alphabet &);
  void deleteSequences(Vector<Sequence*> *);
  void updateCounts(Alignment &,int &gapOpenings,int &gapExtensions,
		    int &totalPairs);
  void initGraphBuilder(ConfigFile &configFile);
  void addSignalDetector(String modelTag,
			 ModelLoader &loader,
			 SignalType signalType,
			 String consensusTag,
			 ConfigFile &configFile,
			 SignalDetector *&detector,
			 SignalDetector *&minusDetector);
  void getMarginals(Alignment &alignment,Array1D<double> &marginals,
		    int &numGapOpenings,int &numGapExtensions,int &sampleSize,
		    int &indels);
  void computeLogOdds();
  void initMatrix();
  ORF *sampleORFs(VertexList &,int numVertices);
  Sequence *getProtein(ORF &,const String &substrate);
  VertexList *getRightEnds(ParseGraph &);
};


#endif
