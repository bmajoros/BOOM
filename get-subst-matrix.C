/****************************************************************
 get-subst-matrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 Infers an amino-acid substitution matrix by observing substitution
 frequencies in real orthologues vs. in randomly selected, aligned
 ORFs.  Also outputs gap open and extension penalties.  Matrix
 entries are true log likelihood ratios, and are not scaled nor
 rounded as are the BLOSUM and PAM matrices.  The same is true of 
 the gap penalties.  This is useful when incorporating alignment
 scores into probabilistic models such as syntenic genefinders, 
 to avoid biasing toward longer/shorter or more/fewer predictions.
 ****************************************************************/
#include "get-subst-matrix.H"
using namespace BOOM;

int frame; // needed by some of exonomy's classes
Alphabet alphabet; // needed by some of exonomy's classes

int main(int argc,char *argv[])
  {
    try
      {
	Application app;
	return app.main(argc,argv);
      }
    catch(const char *p)
      {
	cerr << p << endl;
      }
    catch(const string &msg)
      {
	cerr << msg.c_str() << endl;
      }
    catch(const exception &e)
      {
	cerr << "STL exception caught in main:\n" << e.what() << endl;
      }
    catch(...)
      {
	cerr << "Unknown exception caught in main" << endl;
      }
    return -1;
  }



Application::Application()
  {
    // ctor
  }



int Application::main(int argc,char *argv[])
  {
    // Process command line
    CommandLine cmd(argc,argv,"q");
    if(cmd.numArgs()!=8)
      throw string(
"get-subst-matrix <species1> <species2> <dna1> <dna2> <blosum50>\n\
                 <open> <extend> <*.cfg> [-q]\n\n\
where:\n\
  <species1/2> are orthologous protein multiFASTA files (identically ordered)\n\
  <dna1/2> are long nucleotide (uni-) FASTA files from each species\n\
  <blosum50> is a standard blosum50 matrix (for ortholog alignment)\n\
  <open> is the gap open penalty for use with the blosum50 matrix\n\
  <extend> is the gap extension penalty for use with the blosum50 matrix\n\
  <*.cfg> is a configuration file used for finding ORFs\n\
\n\
  (blosum50 is used only to align orthologues before collecting stats)\n\
  -q = quantize matrix to have integer entries\n\
\n");
    String proteinFile1=cmd.arg(0);
    String proteinFile2=cmd.arg(1);
    String dnaFile1=cmd.arg(2);
    String dnaFile2=cmd.arg(3);
    String blosumFile=cmd.arg(4);
    double blosumGapOpen=cmd.arg(5).asDouble();
    double blosumGapExtend=cmd.arg(6).asDouble();
    String configFilename=cmd.arg(7);
    infile1=proteinFile1;
    infile2=proteinFile2;
    shouldQuantize=cmd.option('q');

    if(blosumGapOpen>0) blosumGapOpen*=-1;
    if(blosumGapExtend>0) blosumGapExtend*=-1;

    cerr << "performing initialization..." << endl; 
    alphabet=DnaAlphabet::global;
    randomize();
    initMatrix();
    SubstitutionMatrix blosumMatrix(blosumFile,AminoAlphabet::global);
    ConfigFile configFile(configFilename);
    initGraphBuilder(configFile);

    cerr << "processing orthologues..." << endl;
    processOrthologues(proteinFile1,proteinFile2,blosumMatrix,
		       blosumGapOpen,blosumGapExtend);

    cerr << "aligning random ORFs..." << endl;
    processDNA(dnaFile1,dnaFile2,blosumMatrix,blosumGapOpen,
	       blosumGapExtend);

    cerr << "finalizing matrix..." << endl;
    computeLogOdds();
    produceOutput();

    return 0;
  }



void Application::initMatrix()
{
  int alphabetSize=AminoAlphabet::global.getNumElements();
  matrix=new TriangMatrix<double>(alphabetSize);
  TriangMatrix<double> &m=*matrix;
  for(int i=0 ; i<alphabetSize ; ++i)
    for(int j=i ; j<alphabetSize ; ++j)
      m(i,j)=0.1; // ### pseudocounts
}



void Application::processOrthologues(const String &proteinFile1,
				     const String &proteinFile2,
				     SubstitutionMatrix &blosumMatrix,
				     double blosumGapOpen,
				     double blosumGapExtend)
{
  // Load proteins
  Vector<Sequence*> &proteins1=
    *loadSequences(proteinFile1,AminoAlphabet::global);
  Vector<Sequence*> &proteins2=
    *loadSequences(proteinFile2,AminoAlphabet::global);
  int numProteins1=proteins1.size(), numProteins2=proteins2.size();
  if(numProteins1!=numProteins2)
    throw String("Different number of proteins in orthologous protein files: ")+numProteins1+" in "+proteinFile1+" vs. "+numProteins2+" in "+proteinFile2;

  // Process each orthologous pair
  int gapOpenings=0, gapExtensions=0, totalPairs=0;
  for(int i=0 ; i<numProteins1 ; ++i)
    {
      Sequence &protein1=*proteins1[i], &protein2=*proteins2[i];
      if(protein1.getLength()<2 || protein2.getLength()<2) continue;
      Needleman aligner(AminoAlphabet::global,protein1,protein2,blosumMatrix,
			blosumGapOpen,blosumGapExtend);
      Alignment *alignment=aligner.fullAlignment();
      updateCounts(*alignment,gapOpenings,gapExtensions,totalPairs);
      delete alignment;
    }

  // Convert counts to probabilities
  int alphabetSize=AminoAlphabet::global.getNumElements();
  TriangMatrix<double> &matrix=*this->matrix;
  double totalPairsDbl=double(totalPairs);
  double sampleSize=0;
  for(int i=0 ; i<alphabetSize ; ++i)
    for(int j=i ; j<alphabetSize ; ++j)
      sampleSize+=matrix(i,j);
  for(int i=0 ; i<alphabetSize ; ++i)
    for(int j=i ; j<alphabetSize ; ++j)
      matrix(i,j)/=sampleSize;
  gapOpenPenalty=gapOpenings/totalPairsDbl;
  gapExtensionPenalty=gapExtensions/totalPairsDbl;

  // Clean up
  deleteSequences(&proteins1);
  deleteSequences(&proteins2);
}



void Application::deleteSequences(Vector<Sequence*> *v)
{
  Vector<Sequence*>::iterator cur=v->begin(), end=v->end();
  for(; cur!=end ; ++cur)
    delete *cur;
  delete v;
}



Vector<Sequence*> *Application::loadSequences(const String &filename,
						  Alphabet &alphabet)
{
  Vector<Sequence*> *v=new Vector<Sequence*>;
  FastaReader reader(filename,AminoAlphabet::global);
  String defline, sequence;
  while(reader.nextSequence(defline,sequence))
    v->push_back(new Sequence(sequence,alphabet));
  return v;
}



void Application::updateCounts(Alignment &alignment,int &gapOpenings,
			       int &gapExtensions,
			       int &totalPairs)
{
  const Sequence &s1=alignment.getFirstSequence();
  const Sequence &s2=alignment.getSecondSequence();
  int len=alignment.getAlignmentLength();
  int index1=0, index2=0;
  bool justMatched=false;
  TriangMatrix<double> &matrix=*this->matrix;
  for(int i=0 ; i<len ; ++i)
    {
      switch(alignment[i])
	{
	case Alignment::MATCH:
	  {
	    Symbol symbol1=s1[index1], symbol2=s2[index2];
	    if(symbol1<=symbol2) 
	      ++matrix(symbol1,symbol2);
	    else 
	      ++matrix(symbol2,symbol1);
	    ++index1;
	    ++index2;
	    justMatched=true;
	  }
	  break;
	case Alignment::FIRST_UNMATCHED:
	  if(justMatched) 
	    {
	      ++gapOpenings;
	      justMatched=false;
	    }
	  else ++gapExtensions;
	  ++index1;
	  break;
	case Alignment::SECOND_UNMATCHED:
	  if(justMatched) 
	    {
	      ++gapOpenings;
	      justMatched=false;
	    }
	  else ++gapExtensions;
	  ++index2;
	  break;
	}
    }
  totalPairs+=len; // ### ?
}



void Application::processDNA(const String &dnaFile1,
			     const String &dnaFile2,
			     SubstitutionMatrix &blosumMatrix,
			     double blosumGapOpen,double blosumGapExtend)
{
  // Load DNA sequences
  cerr<<"\tloading sequences..."<<endl;
  Sequence &seq1=*Sequence::load(dnaFile1,DnaAlphabet::global);
  Sequence &seq2=*Sequence::load(dnaFile2,DnaAlphabet::global);
  String *string1=seq1.toString(DnaAlphabet::global);
  String *string2=seq2.toString(DnaAlphabet::global);

  // Build parse graphs to identify all ORFs
  cerr<<"\tbuilding & reducing parse graphs..."<<endl;
  cerr << "\t\tbuilding graph 1" << endl;
  ParseGraph *graph1=
    graphBuilder->buildParseGraph(seq1,seq1.getLength(),string1->c_str());
  cerr << "\t\treducing graph 1" << endl;
  reducer.reduceGraph(*graph1);
  cerr << "\t\tbuilding graph 2" << endl;
  ParseGraph *graph2=
    graphBuilder->buildParseGraph(seq2,seq2.getLength(),string2->c_str());
  cerr << "\t\treducing graph 2" << endl;
  reducer.reduceGraph(*graph2);

  cerr<<"\tgetting randomization lists..."<<endl;
  VertexList *rightEnds1=getRightEnds(*graph1);
  VertexList *rightEnds2=getRightEnds(*graph2);
  int numRightEnds1=rightEnds1->size();
  int numRightEnds2=rightEnds2->size();

  // Iteratively choose random ORF pairs and align their proteins
  int alphabetSize=AminoAlphabet::global.getNumElements();
  marginals=new Array1D<double>(alphabetSize);
  marginals->setAllTo(0.0);
  int numGapOpenings=0, numGapExtensions=0, sampleSize=0, indels=0;
  for(int i=0 ; i<NUM_ORF_PAIRS ; ++i)
    {
      ORF *orf1=sampleORFs(*rightEnds1,numRightEnds1);
      ORF *orf2=sampleORFs(*rightEnds2,numRightEnds2);
      Sequence *protein1=getProtein(*orf1,*string1);
      Sequence *protein2=getProtein(*orf2,*string2);
      Needleman aligner(AminoAlphabet::global,*protein1,*protein2,blosumMatrix,
			blosumGapOpen,blosumGapExtend);
      Alignment *alignment=aligner.fullAlignment();
      //cout << "\nRANDOM ALIGNMENT:\n" << *alignment << endl << endl;//###
      getMarginals(*alignment,*marginals,numGapOpenings,numGapExtensions,
		   sampleSize,indels);

      delete alignment;
      delete protein1;
      delete protein2;
      delete orf1;
      delete orf2;
    }

  for(int i=0 ; i<alphabetSize ; ++i)
    {
      (*marginals)[i]/=sampleSize;
      //cout << "MARGINAL " << i << "=" << (*marginals)[i] << endl;
    }
  randomOpenPenalty=numGapOpenings/double(sampleSize+indels);
  randomExtensionPenalty=numGapExtensions/double(sampleSize+indels);

  // Clean up
  delete graph1;
  delete graph2;
  delete &seq1;
  delete &seq2;
  delete string1;
  delete string2;
}



void Application::initGraphBuilder(ConfigFile &configFile)
{
  SignalDetector *startCodonDetector, *startCodonMinusDetector,
    *stopCodonDetector, *stopCodonMinusDetector, *donorDetector,
    *donorMinusDetector, *acceptorDetector, *acceptorMinusDetector;
  ModelLoader modelLoader(DnaAlphabet::global);

  // =======================
  // Load the signal sensors
  // =======================
  addSignalDetector("start-codons",modelLoader,START_CODON,"start-consensus",
		    configFile,startCodonDetector,startCodonMinusDetector);
  addSignalDetector("stop-codons",modelLoader,STOP_CODON,"stop-consensus",
		    configFile,stopCodonDetector,stopCodonMinusDetector);
  addSignalDetector("donors",modelLoader,DONOR,"donor-consensus",
		    configFile,donorDetector,donorMinusDetector);
  addSignalDetector("acceptors",modelLoader,ACCEPTOR,"acceptor-consensus",
		    configFile,acceptorDetector,acceptorMinusDetector);

  // =======================================================
  // Add a pseudo-stop-codon detector, for frame termination
  // =======================================================

  SignalDetector *posDetector=new PseudoStopDetector;
  SignalDetector *negDetector=new PseudoStopDetector(NEG_PSEUDO_STOP);
  String consensusString=configFile.lookupOrDie("stop-consensus");
  Vector<String> &consensusStrings=*consensusString.getFields(", \t");
  int n=consensusStrings.size();
  for(int i=0 ; i<n ; ++i)
    {
      Sequence *forwardConsensus=new Sequence(consensusStrings[i],alphabet);
      posDetector->addConsensus(forwardConsensus);
      Sequence *reverseConsensus=
	forwardConsensus->reverseComplement(alphabet);
      negDetector->addConsensus(reverseConsensus);
    }
  delete &consensusStrings;
  signalDetectors.push_back(posDetector);
  signalDetectors.push_back(negDetector);

  graphBuilder=new GraphBuilder(signalDetectors,LOCAL_OPTIMALITY_DISTANCE);
}



void Application::addSignalDetector(String modelTag,
				    ModelLoader &loader,
				    SignalType signalType,
				    String consensusTag,
				    ConfigFile &configFile,
				    SignalDetector *&detector,
				    SignalDetector *&minusDetector)
{
  // Create SignalDetector for the plus strand:
  String modelFile=configFile.lookupOrDie(modelTag);

  SignalModel *submodel=
    dynamic_cast<SignalModel*>(loader.loadModel(modelFile));
  if(!submodel) throw modelFile+" does not contain a SignalModel object";
  detector=new SignalDetector(submodel,signalType);
  signalDetectors.push_back(detector);

  // Now create an analogous SignalDetector for the minus strand:
  SignalModel *minusModel=
    dynamic_cast<SignalModel*>(submodel->reverseComplement());
  if(!minusModel) throw "reverseComplement returns NULL";//###
  minusDetector=
    new SignalDetector(minusModel,reverseSignal(signalType));
  signalDetectors.push_back(minusDetector);

  // Add consensus sequences to plus and minus strand detectors:
  String consensusString=configFile.lookupOrDie(consensusTag);
  Vector<String> &consensusStrings=*consensusString.getFields(", \t");
  int n=consensusStrings.size();
  for(int i=0 ; i<n ; ++i)
    {
      Sequence *consensus=new Sequence(consensusStrings[i],alphabet);
      detector->addConsensus(consensus);
      minusDetector->addConsensus(consensus->reverseComplement(alphabet));
    }

  delete &consensusStrings;
}



void Application::getMarginals(Alignment &alignment,
			       Array1D<double> &marginals,
			       int &gapOpenings,int &gapExtensions,
			       int &sampleSize,int &indels)
{
  const Sequence &s1=alignment.getFirstSequence();
  const Sequence &s2=alignment.getSecondSequence();
  int len=alignment.getAlignmentLength();
  int index1=0, index2=0;
  bool justMatched=false;
  for(int i=0 ; i<len ; ++i)
    {
      switch(alignment[i])
	{
	case Alignment::MATCH:
	  ++marginals[s1[index1]];
	  ++marginals[s2[index2]];
	  sampleSize+=2;
	  ++index1;
	  ++index2;
	  justMatched=true;
	  break;
	case Alignment::FIRST_UNMATCHED:
	  if(justMatched) 
	    {
	      ++gapOpenings;
	      justMatched=false;
	    }
	  else ++gapExtensions;
	  ++index1;
	  ++indels;
	  break;
	case Alignment::SECOND_UNMATCHED:
	  if(justMatched) 
	    {
	      ++gapOpenings;
	      justMatched=false;
	    }
	  else ++gapExtensions;
	  ++index2;
	  ++indels;
	  break;
	}
    }
}



void Application::computeLogOdds()
{
  TriangMatrix<double> &m=*matrix;
  Array1D<double> &marginals=*this->marginals;
  int alphabetSize=AminoAlphabet::global.getNumElements();
  for(int i=0 ; i<alphabetSize ; ++i)
    for(int j=i ; j<alphabetSize ; ++j)
      {
	double marginalI=marginals[i], marginalJ=marginals[j];
	double &matrixEntry=m(i,j);
	//cout<<i<<" vs "<<j<<": "<<matrixEntry<<"/("<<marginalI<<"*"<<marginalJ<<")"<<endl;
	matrixEntry=log(matrixEntry/(marginalI*marginalJ));	
      }

  // Quantize to integers if the user requested it
  if(shouldQuantize)
    for(int i=0 ; i<alphabetSize ; ++i)
      for(int j=i ; j<alphabetSize ; ++j)
	m(i,j)=int(m(i,j));

  gapOpenPenalty=log(gapOpenPenalty/randomOpenPenalty);
  gapExtensionPenalty=log(gapExtensionPenalty/randomExtensionPenalty);
}


void Application::produceOutput()
{
  cout << "# Twain/get-subst-matrix" << endl;
  cout << "# executed on " << getDateAndTime();
  cout << "# input file 1: " << infile1 << endl;
  cout << "# input file 2: " << infile2 << endl;
  cout << "# matrix entries expressed in log odds" << endl;
  cout << "# recommended gap open penalty: " << gapOpenPenalty << endl;
  cout << "# recommended gap extension penalty: " 
       << gapExtensionPenalty << endl;

  TriangMatrix<double> &m=*matrix;
  int alphabetSize=AminoAlphabet::global.getNumElements();
  cout << "  ";
  for(int i=0 ; i<alphabetSize ; ++i)
    cout << AminoAlphabet::global.lookup(i) << "  ";
  cout << endl;
  for(int i=0 ; i<alphabetSize ; ++i)
    {
      cout << AminoAlphabet::global.lookup(i) << '\t';
      for(int j=0 ; j<alphabetSize ; ++j)
	{
	  double value=(i<=j ? m(i,j) : m(j,i));
	  cout << value << "  ";
	}
      cout << endl;
    }
}



Sequence *Application::getProtein(ORF &orf,const String &substrate)
{
  String transcript;
  ORF::iterator cur=orf.begin(), end=orf.end();
  for(; cur!=end ; ++cur)
    {
      ParseEdge &edge=**cur;
      if(edge.isCoding())
	{
	  int begin=edge.getBeginCoordinate();
	  int end=edge.getEndCoordinate();
	  String exon=substrate.substring(begin,end-begin);
	  transcript+=exon;
	}
    }
  return new Sequence(ProteinTrans::translate(transcript),
		      AminoAlphabet::global);
}



VertexList *Application::getRightEnds(ParseGraph &graph)
{
  VertexList *list=new VertexList;
  int numVertices=graph.numVertices();
  for(int i=0 ; i<numVertices ; ++i)
    {
      ParseVertex &vertex=graph.getVertex(i);
      switch(vertex.getSignalType())
	{
	case STOP_CODON:
	case NEG_START_CODON:
	  list->push_back(&vertex);
	  break;
	}
    }
  return list;
}



ORF *Application::sampleORFs(VertexList &rightEnds,int numVertices)
{
  // Randomly select a vertex to end the ORF
  ORF *orf=new ORF;
  int index=RandomNumber(numVertices);
  ParseVertex *vertex=rightEnds[index];

  // Iteratively construct the ORF right-to-left
  int frame=(vertex->getStrand()==PLUS_STRAND ? 0 : 2);
  while(1)
    {
      Vector<ParseEdge*> &edgesIn=vertex->getEdgesIn();
      int numEdges=edgesIn.size();

      // Get the set of edges which have valid frames
      Vector<ParseEdge*> validEdges;
      for(int i=0 ; i<numEdges ; ++i)
	{
	  ParseEdge *edge=edgesIn[i];
	  if(edge->getFrames().isMember(frame) &&
	     edge->getLeft().getSignalType()!=LEFT_TERMINUS) 
	    validEdges.push_back(edge);
	}
      if(validEdges.size()==0)
	{
	  orf->clear();
	  vertex=rightEnds[RandomNumber(numVertices)];
	  frame=(vertex->getStrand()==PLUS_STRAND ? 0 : 2);
	  continue;
	}

      // Randomly choose a valid edge and traverse it leftward
      ParseEdge *edge=
	validEdges[RandomNumber(validEdges.size())];
      vertex=&edge->getLeft();
      if(edge->isCoding())
	frame=edge->propagateFrameBackward(frame);
      orf->push_front(edge);

      // Stop when the left end of an ORF is reached
      switch(vertex->getSignalType())
	{
	case START_CODON:
	case NEG_STOP_CODON:
	  return orf;
	case LEFT_TERMINUS:
	  orf->clear();
	  vertex=rightEnds[RandomNumber(numVertices)];
	  frame=(vertex->getStrand()==PLUS_STRAND ? 0 : 2);
	  break;
	}
    }
}



