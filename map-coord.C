/****************************************************************
 map-coord.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "BOOM/String.H"
#include "BOOM/CommandLine.H"
#include "BOOM/CigarString.H"
using namespace std;
using namespace BOOM;

class Application {
public:
  Application();
  int main(int argc,char *argv[]);
};


int main(int argc,char *argv[])
{
  try {
    Application app;
    return app.main(argc,argv);
  }
  catch(const char *p) { cerr << p << endl; }
  catch(const string &msg) { cerr << msg.c_str() << endl; }
  catch(const exception &e)
    {cerr << "STL exception caught in main:\n" << e.what() << endl;}
  catch(...) { cerr << "Unknown exception caught in main" << endl; }
  return -1;
}



Application::Application()
{
  // ctor
}



int Application::main(int argc,char *argv[])
{
  // Process command line
  CommandLine cmd(argc,argv,"a");
  if(cmd.numArgs()!=2)
    throw String("\n\
map-coord <*.cigar> <coord>\n\
   -a = perform approximate mapping if the location is deleted\n\
");
  const String cigarFile=cmd.arg(0);
  const int coord=cmd.arg(1).asInt();
  const bool wantApprox=cmd.option('a');

  CigarString cigar; cigar.load(cigarFile);
  CigarAlignment &alignment=*cigar.getAlignment();
  if(coord>alignment.length()) throw "out of range";
  int mapped=alignment[coord];
  if(mapped==CIGAR_UNDEFINED && wantApprox) {
    const int L=alignment.length();
    for(int delta=0 ; delta<L ; ++delta) {
      int plus=coord+delta, minus=coord-delta;
      if(minus>=0 && alignment[minus]!=CIGAR_UNDEFINED)
	{ mapped=alignment[minus]; break;}
      if(plus<L && alignment[plus]!=CIGAR_UNDEFINED)
	{ mapped=alignment[plus]; break; }
    }
  }
  cout<<mapped<<endl;

  return 0;
}

