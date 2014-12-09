/*
 TokenStream.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "TokenStream.H"
#include "LambdaException.H"
using namespace BOOM;
using namespace Lambda;


TokenStream::TokenStream(Scanner *scanner)
  : scanner(*scanner), pushedBack(NULL)
{
  // ctor
}



TokenStream::TokenStream(istream &is)
  : scanner(*new Scanner(is)), pushedBack(NULL)
{
  // ctor
}



TokenStream::~TokenStream()
{
  // dtor

  delete &scanner;
}



TokenType TokenStream::peekTokenType()
{
  if(!pushedBack)
    pushedBack=scanner.nextToken();

  return pushedBack->getTokenType();
}



Token *TokenStream::nextToken()
{
  Token *retval;

  if(pushedBack)
    {
      retval=pushedBack;
      pushedBack=NULL;
    }
  else
    retval=scanner.nextToken();

  return retval;
}



void TokenStream::pushBack(Token *token)
{
  if(pushedBack)
    throw LambdaException("Pushing back more than one token in TokenStream.C");
  
  pushedBack=token;
}





