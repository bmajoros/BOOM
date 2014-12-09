/****************************************************************
 random.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.

 Uses the Mersenne Twister random number generator, which is a
 widely accepted standard for numerical simulations.
 ****************************************************************/
#include <iostream>
#include <sys/time.h>
#include "String.H"
#include "CommandLine.H"
#include "GSL/Random.H"
#include "Constants.H"
using namespace std;
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
    CommandLine cmd(argc,argv,"s:n:h");
    if(cmd.numArgs()>0 || cmd.option('h')) 
      throw String(
"\n\
random [options]\n\
\n\
   Uses the Mersenne Twister random number generator, which is a\n\
   widely accepted standard for numerical simulations.\n\
\n\
 options:\n\
  -s seed : seed the generator with the given positive integer\n\
  -n number : generate a block of n random numbers\n\
\n\
");
    if(cmd.option('s')) GSL::Random::seed(cmd.optParm('s').asLong());
    else GSL::Random::seed(time(0));
    int n=cmd.option('n') ? cmd.optParm('n').asInt() : 1;
    for(int i=0 ; i<n ; ++i) {
      unsigned long int ul=GSL::Random::randomUnsignedLong(0,LARGEST_LONG_INT);
      long l=(long) ul;
      cout<<l<<endl;
    }
    return 0;
  }

