/*
 TestParser.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Parser.H"
#include "LambdaException.H"
#include "AstPrinter.H"
#include <strstream.h>

int main(int,char**)
{
  char command[1000];

  try
    {
      RunTimeEnvironment env;
      while(!cin.eof())
	{
	  cout << ">";
	  cout.flush();

	  cin.getline(command,sizeof(command));

	  strstream str;
	  str << command << ends;
	  Parser parser(str,env);

	  AstForest *forest=parser.parse();
	  cout << "ok" << endl;
      
	  AstPrinter astPrinter(cout);
	  astPrinter.visit(*forest);
	  cout << endl;
	  cout << "ok" << endl;
	}
    }
  catch(const RootException &e)
    {
      cout << "Exception: " << e.GetMessage() << endl;
    }
}
