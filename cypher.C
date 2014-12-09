/* 
 cypher.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include <string>
#include <iostream>
#include "CommandLine.H"
#include "Cypher.H"
using namespace std;

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
    BOOM::CommandLine cmd(argc,argv,"");
    if(cmd.numArgs()!=2)
      throw string("cypher <key> <string>");
    const BOOM::String &key=cmd.arg(0);
    const BOOM::String &in=cmd.arg(1);
    cout<<"key=\""<<key<<"\""<<endl;
    cout<<"in=\""<<in<<"\""<<endl;
    Cypher c(key);
    BOOM::String out=c(in);
    BOOM::String uncyphered=c(out);
    cout<<"encoded=\""<<out<<"\", length="<<out.length()<<endl;
    cout<<"reconstructed=\""<<uncyphered<<"\""<<endl;

    return 0;
  }

