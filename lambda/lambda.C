/*
 lambda.C : command-line interface to interpreter
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include <sstream>
#include <fstream>
#include <math.h>
#include "BOOM/Random.H"
#include "lambda.H"
#include "Parser.H"
#include "LambdaException.H"
#include "AstPrinter.H"
#include "ForeignFunctionObject.H"
#include "ObjectPrinter.H"
#include "Evaluator.H"
#include "Lists.H"
#include "LambdaVector.H"
#include "globals.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


int main(int argc,char *argv[])
{
  randomize();

  char command[1000];

  try
    {
      LambdaCmdLine commandLine(argc,argv);
      global_commandLine=&commandLine;

      RunTimeEnvironment env;
      Evaluator evaluator(env);
      env.registerMarkHook(&evaluator);
      global_evaluator=&evaluator;
      ObjectPrinter printer(env);

      // Create built-in objects
      defineGlobals(env);
      
      // Autoload any files given on command line
      if(commandLine.getNumFilenames()>0)
	{
	  int i;
	  for(i=0 ; i<commandLine.getNumFilenames() ; ++i)
	    LoadSource(commandLine.getIthFilename(i).c_str(),
		       env);
	  exit(0);
	}

      cout << "Lambda Interpreter v1.0" << endl;
      cout << "Bill Majoros, June 1997\n" << endl;

      // Read-eval-print loop:
      while(!cin.eof())
	{
	  // Display a command-line prompt
	  cout << ">";
	  cout.flush();

	  // Read an expression from the user
	  cin.getline(command,sizeof(command));
	  if(!strcmp(command,"exit") || !strcmp(command,"quit"))
	    exit(0);

	  try
	    {
	      // Parse
	      istringstream str(command);
	      Parser parser(str,env);
	      AstForest *forest=parser.parse();
	     
	      // For each expression entered on the command line...
	      int i;
	      for(i=0 ; i<forest->getNumTrees() ; ++i)
		{
		  // Evaluate the expression
		  LambdaObject *value=
		    evaluator.evaluateTopLevel(
		       forest->getIthTree(i));
		  
		  // Print the resulting value
		  printer.print(value,cout);
		  cout << endl;

		  // Run the garbage-collector
		  env.runGC();
		}
	    }
	  catch(const RootException &e)
	    {
	      cout << "Exception: " << e.getMessage() << endl;
	      env.cleanStack(); // so we can resume normally
	    }
	}
    }
  catch(const RootException &e)
    {
      cout << "Exception: " << e.getMessage() << endl;
    }
}



