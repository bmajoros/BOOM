/****************************************************************
 GenericTokenStream.C
 Copyright (C)2014 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "GenericTokenStream.H"
using namespace std;
using namespace BOOM;


GenericTokenStream::GenericTokenStream(GenericScanner *scanner)
  : scanner(*scanner), pushedBack(NULL)
{
  // ctor
}



GenericTokenStream::GenericTokenStream(istream &is)
  : scanner(*new GenericScanner(is)), pushedBack(NULL)
{
  // ctor
}



GenericTokenType GenericTokenStream::peekTokenType()
{
  if(!pushedBack) pushedBack=scanner.nextToken();
  return pushedBack->getTokenType();
}



GenericToken *GenericTokenStream::nextToken()
{
  GenericToken *retval;
  if(pushedBack) {
    retval=pushedBack;
    pushedBack=NULL;
  }
  else retval=scanner.nextToken();
  return retval;
}



void GenericTokenStream::pushBack(GenericToken *token)
{
  if(pushedBack)
    throw RootException("Pushing back more than one token in GenericTokenStream.C");
  
  pushedBack=token;
}



GenericScanner &GenericTokenStream::getScanner()
{
  return scanner;
}


