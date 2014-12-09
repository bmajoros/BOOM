/*
 testapi.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "testapi.H"
#include "lambda.H"
#include "Parser.H"
#include "LambdaException.H"
#include "AstPrinter.H"
#include <strstream.h>
#include "ForeignFunctionObject.H"
#include "ObjectPrinter.H"
#include "Evaluator.H"
#include <fstream.h>
#include "Lists.H"
#include <stdlib.h>
#include <math.h>
#include "LambdaVector.H"
#include "BOOM/Random.H"
#include "globals.H"
#include "LambdaAPI.H"

int main(int, char**)
{
  LambdaAPI api;
  ObjectPrinter objectPrinter(api.getEnvironment());
  AstPrinter astPrinter(cout,api.getEnvironment());

  AstCombination *combo=
    api.makeComboWithArity(api.makeGlobalRef("sqrt",0,false),1,
      false);
  api.setArg(combo,0,api.makeInt(2,false));

  astPrinter.print(combo);

  LambdaObject *result=api.evaluate(combo);
  objectPrinter.print(result,cout);

  return 0;
}







