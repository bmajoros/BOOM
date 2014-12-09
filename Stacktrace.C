/***********************************************************************
 Stacktrace.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include <iostream>
#include "Stacktrace.H"
using namespace std;



BOOM::Stacktrace::Stacktrace(const char *message)
{
  cout << endl << message << endl << endl;
  cout << "Initiating core dump...use your debugger to get a stack trace"
       << endl;
  ((Crasher*)0)->crash();
}



BOOM::Stacktrace::Stacktrace(const BOOM::String &message)
{
  cout << endl << message << endl << endl;
  cout << "Initiating core dump...use your debugger to get a stack trace"
       << endl;
  ((Crasher*)0)->crash();
}
