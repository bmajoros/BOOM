
/*
 rapid-comparison.C
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
#include "RapidSequenceComparison.H"
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
    CommandLine cmd(argc,argv,"n:");
    if(cmd.numArgs()!=4)
      throw string("rapid-comparison <SubstitutionMatrix> <*.fasta> <*.fasta> DNA|PROTEIN\n\n\
example: rapid-comparison blosum62 5 2 1.fasta 2.fasta PROTEIN\n");
    String matrixFile=cmd.arg(0);
    String file1=cmd.arg(1);
    String file2=cmd.arg(2);
    String type=cmd.arg(3);
    if(type!="DNA" && type!="PROTEIN") throw "specify DNA or PROTEIN";

    Alphabet &alphabet=
      (type=="DNA") ? 
      static_cast<Alphabet&>(DnaAlphabet::global) : 
      static_cast<Alphabet&>(AminoAlphabet::global);
    SubstitutionMatrix<float> M(matrixFile,alphabet);
    Sequence *seq1=Sequence::load(file1,alphabet);
    Sequence *seq2=Sequence::load(file2,alphabet);

    int n=cmd.option('n') ? cmd.optParm('n').asInt() : 1;
    for(int i=0 ; i<n ; ++i)
      {
	RapidSequenceComparison<float> aligner(alphabet,M);
	float score=aligner.compare(*seq1,*seq2);
	cout<<"score="<<score<<endl;
      }

    return 0;
  }

