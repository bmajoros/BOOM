/*
 globals.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_globals_H
#define INCL_globals_H

#include "Parser.H"
#include "LambdaException.H"
#include "AstPrinter.H"
#include <sstream>
#include "ForeignFunctionObject.H"
#include "ObjectPrinter.H"
#include "Evaluator.H"
#include <fstream>
#include "Lists.H"
#include <math.h>
#include "LambdaVector.H"
#include "BOOM/Random.H"
#include "CommandLine.H"
using namespace std;
using namespace BOOM;

namespace Lambda {

extern LambdaObject *true_object;
extern Evaluator *global_evaluator;
extern LambdaCmdLine *global_commandLine;


void defineGlobals(RunTimeEnvironment &);
void declareBuiltInGlobals(RunTimeEnvironment &env);
void defineBuiltInGlobals(RunTimeEnvironment &env);
LambdaFloat *toLambdaFloat(LambdaObject *intOrFloat,
			   RunTimeEnvironment &env);
void backpatch(LambdaSymTabNode *symTabNode,RunTimeEnvironment &env);
void LoadSource(const String &filename,RunTimeEnvironment &env);

}

#endif
