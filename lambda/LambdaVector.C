/*
 LambdaVector.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaVector.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


LambdaVector::LambdaVector(int size)
  : array(size), LambdaObject(OBTYPE_VECTOR)
{
  // ctor

  int i;
  for(i=0 ; i<size ; ++i)
    array[i]=NULL;
}



int LambdaVector::getSize() const
{
  return array.size();
}



LambdaObject *LambdaVector::getElement(int i)
{
  return (LambdaObject*) array[i];
}



void LambdaVector::setElement(int i,LambdaObject *object)
{
  array[i]=object;
}



void LambdaVector::pushAccessibles(MarkStack &markStack)
{
  int i;
  for(i=0 ; i<array.size() ; ++i)
    markStack.push((LambdaObject*) array[i]);
}


