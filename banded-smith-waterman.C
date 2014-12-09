
/* 
 banded-smith-waterman.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/

#include <string>
#include <iostream>
#include <math.h>
#include "CommandLine.H"
#include "FastaReader.H"
#include "SubstitutionMatrix.H"
#include "AminoAlphabet.H"
#include "DnaAlphabet.H"
#include "BandedSmithWaterman.H"
using namespace BOOM;


class Application
{
public:
  Application();
  int main(int argc,char *argv[]);
};


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
    CommandLine cmd(argc,argv,"qn:w:r:");
    if(cmd.numArgs()!=7)
      throw string(
"\nbanded-smith-waterman <SubstitutionMatrix> <+GapOpenPenalty> \n\
                       <+GapExtendPenalty> <*.fasta> <*.fasta> \n\
                       DNA|PROTEIN <band-width> [-q] [-w #]\n\n\
example: banded-smith-waterman blosum62 5 2 1.fasta 2.fasta DNA 5\n\n\
             -q = quiet (no alignment output)\n\
             -w # = format output to given width (default is 60)\n\
             -r M = recommend band width for M bytes of RAM\n\n");
    String matrixFile=cmd.arg(0);
    double gapOpen=-fabs(cmd.arg(1).asDouble());
    double gapExtend=-fabs(cmd.arg(2).asDouble());
    String file1=cmd.arg(3);
    String file2=cmd.arg(4);
    String type=cmd.arg(5);
    int bandWidth=cmd.arg(6).asInt();
    if(type!="DNA" && type!="PROTEIN") throw "specify DNA or PROTEIN";
    bool quiet=cmd.option('q');
    if(cmd.option('w')) Alignment::MAX_WIDTH=cmd.optParm('w').asInt();

    Alphabet &alphabet=
      (type=="DNA") ? 
      static_cast<Alphabet&>(DnaAlphabet::global) : 
      static_cast<Alphabet&>(AminoAlphabet::global);
    SubstitutionMatrix<double> M(matrixFile,alphabet);
    Sequence *seq1=Sequence::load(file1,alphabet);
    Sequence *seq2=Sequence::load(file2,alphabet);

    if(cmd.option('r'))
      {
	int maxRAM=cmd.optParm('r').asInt();
	cout<<"Recommended maximum band width: "
	    <<BandedSmithWaterman<double>::recommendBandWidth(maxRAM,
						   seq1->getLength())
	    <<endl;
	exit(0);
      }

    int n=cmd.option('n') ? cmd.optParm('n').asInt() : 1;
    for(int i=0 ; i<n ; ++i)
      {
	BandedSmithWaterman<double> aligner(alphabet,*seq1,*seq2,M,gapOpen,
					    gapExtend,bandWidth);
	Alignment *alignment=aligner.fullAlignment();
	int mismatches, insertions;
	alignment->countMismatches(mismatches,insertions);
	int alignmentLength=alignment->getAlignmentLength();
	int nearMatches=alignment->countNearMatches(M);
	float percentSimilarity=
	  int(1000*nearMatches/float(alignmentLength))/10;
	int matches=alignmentLength-insertions-mismatches;
	float percentIdentity=int(1000*matches/float(alignmentLength))/10;
	double score=alignment->getScore();
	if(quiet) cout<<"score="<<score<<endl;
	else
	  {
	    cout << "\n-----------------------------------------------------------------------\n";
	    cout << "Sequence #1: " << file1 << endl;
	    cout << "Sequence #2: " << file2 << endl;
	    cout << "Percent identity: " << percentIdentity
		 << "%, matches=" << matches << ", mismatches="<< mismatches
		 << "\nPercent similarity: "<<percentSimilarity<<"%"
		 << ",\ninsertions=" << insertions
		 << ", alignment length=" << alignmentLength 
		 << ", score=" << score
		 << endl;
	    cout << "\n<alignment>\n\n" << *alignment << "\n</alignment>\n";
	    cout << "-----------------------------------------------------------------------\n";
	  }
      }
    return 0;
  }

