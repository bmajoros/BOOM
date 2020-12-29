
/*
 ungapped-aligner.C
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
#include "UngappedAligner.H"
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
    CommandLine cmd(argc,argv,"s");
    if(cmd.numArgs()!=4)
      throw string("ungapped-aligner [-s] <SubstitutionMatrix> <*.fasta> <*.fasta> DNA|PROTEIN\n\n\
-s = filenames are actually inline sequences\n\
example: ungapped-aligner blosum62 1.fasta 2.fasta DNA|PROTEIN\n");
    String matrixFile=cmd.arg(0);
    String file1=cmd.arg(1);
    String file2=cmd.arg(2);
    String type=cmd.arg(3);
    if(type!="DNA" && type!="PROTEIN") throw "specify DNA or PROTEIN";

    Alphabet &alphabet=
      (type=="DNA") ? 
      static_cast<Alphabet&>(DnaAlphabet::global()) : 
      static_cast<Alphabet&>(AminoAlphabet::global());
    SubstitutionMatrix<float> M(matrixFile,alphabet);

    Sequence *seq1=NULL, *seq2=NULL;
    if(cmd.option('s'))
      { seq1=new Sequence(file1,alphabet); seq2=new Sequence(file2,alphabet); }
    else {
      seq1=Sequence::load(file1,alphabet); seq2=Sequence::load(file2,alphabet);
    }

    UngappedAligner<float> aligner(alphabet,*seq1,*seq2,M);
    float score=aligner.getScore();
    cout<<"score="<<score<<endl;
    return 0;
  }

