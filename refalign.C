
/*
 refalign.C
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
#include "RefAlign.H"
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
    if(cmd.numArgs()!=6)
      throw string(
"\nrefalign <SubstitutionMatrix> <+GapOpenPenalty> <+GapExtendPenalty>\n\
         <*.fasta> DNA|PROTEIN <band-width>\n\n\
example: refalign blosum62 5 2 multi.fasta DNA 5\n\n\
\n\n");
    String matrixFile=cmd.arg(0);
    double gapOpen=-fabs(cmd.arg(1).asDouble());
    double gapExtend=-fabs(cmd.arg(2).asDouble());
    String infile=cmd.arg(3);
    String type=cmd.arg(4);
    int bandWidth=cmd.arg(5).asInt();
    if(type!="DNA" && type!="PROTEIN") throw "specify DNA or PROTEIN";

    Alphabet &alphabet=
      (type=="DNA") ? 
      static_cast<Alphabet&>(DnaAlphabet::global) : 
      static_cast<Alphabet&>(AminoAlphabet::global);
    SubstitutionMatrix<double> M(matrixFile,alphabet);
    RefAlign<double> aligner(alphabet,M,gapOpen,gapExtend,bandWidth);
    FastaReader reader(infile,alphabet);
    String defline, seqStr;
    Vector<String> labels;
    int longestLabel=0;
    while(reader.nextSequence(defline,seqStr))
      {
	String id, remainder;
	FastaReader::parseDefline(defline,id,remainder);
	Sequence *seq=new Sequence(seqStr,alphabet);
	aligner.addSequence(*seq);
	labels.push_back(id);
	int labelLength=id.length();
	if(labelLength>longestLabel) longestLabel=labelLength;
      }
    aligner.performAlignment();
    int numTracks=aligner.numTracks();
    for(int i=0 ; i<numTracks ; ++i)
      {
	String label=labels[i];
	String padding(longestLabel-label.length(),' ');
	label+=padding;
	const String &track=aligner.getIthTrack(i);
	cout<<label<<" "<<track<<endl;
      }

    return 0;
  }

