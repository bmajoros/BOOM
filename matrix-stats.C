/****************************************************************
 matrix-stats.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <string>
#include <iostream>
#include "CommandLine.H"
#include "DnaAlphabet.H"
#include "AminoAlphabet.H"
#include "SubstitutionMatrix.H"
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
    CommandLine cmd(argc,argv,"dp");
    if(cmd.numArgs()!=1)
      throw string("matrix-stats [-d or -p] <*.matrix>\n\
where:\n\
\t-d = DNA\n\
\t-p = protein\n\
\n");
    bool dna=cmd.option('d');
    if(!dna && !cmd.option('p')) throw "-d or -p required (DNA/protein)";
    String filename=cmd.arg(0);

    // Load matrix
    Alphabet &alphabet=
      dna ? 
      (Alphabet&) DnaAlphabet::global :
      (Alphabet&) AminoAlphabet::global;
    SubstitutionMatrix matrix(filename,alphabet);

    // ===============================
    // NON-* AVERAGE OVER ENTIRE TABLE
    // ===============================
    double sum=0;
    int numAlpha=alphabet.getNumElements(), n=0;
    for(Symbol s=0 ; s<numAlpha ; ++s)
      {
	char sChar=alphabet.lookup(s);
	if(sChar=='*' || dna && sChar=='N') continue;
	for(Symbol t=s ; t<numAlpha ; ++t)
	  {
	    sum+=matrix(s,t);
	    ++n;
	  }
      }
    double tableMean=sum/n;

    // =======================================
    // NON-* AVERAGE OVER NONIDENTICAL MATCHES
    // =======================================

    sum=0;
    n=0;
    for(Symbol s=0 ; s<numAlpha ; ++s)
      {
	char sChar=alphabet.lookup(s);
	if(sChar=='*' || dna && sChar=='N') continue;
	for(Symbol t=s+1 ; t<numAlpha ; ++t)
	  {
	    sum+=matrix(s,t);
	    ++n;
	  }
      }
    double mismatchMean=sum/n;

    // ====================================
    // NON-* AVERAGE OVER IDENTICAL MATCHES
    // ====================================

    sum=0;
    n=0;
    for(Symbol s=0 ; s<numAlpha ; ++s)
      {
	char sChar=alphabet.lookup(s);
	if(sChar=='*' || dna && sChar=='N') continue;
	sum+=matrix(s,s);
	++n;
      }
    double matchMean=sum/n;

    cout << "average over entire table (except amino::* and DNA::N): "
	 << tableMean << endl;
    cout << "average over mismatches (except */N): " << mismatchMean << endl;
    cout << "average over matches (except */N): " << matchMean << endl;

    return 0;
  }

