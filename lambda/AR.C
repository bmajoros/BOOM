/*
 AR.C (ActivationRecords)
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "AR.H"
#include "LambdaException.H"
#include <iostream>
#include <sstream>
using namespace std;
using namespace BOOM;
using namespace Lambda;

// ****************************************
//	    ActivationRecord methods
// ****************************************

ActivationRecord::ActivationRecord(int numEntries,
				   ActivationRecord *staticChain)
  : entries(new LambdaObject*[numEntries]), 
    numEntries(numEntries),
    staticChain(staticChain)
{
  // ctor
  
  // Initialize all entries to point to the nil object
  int i;
  for(i=0 ; i<numEntries ; i++)
    entries[i]=NULL;
}



ActivationRecord *ActivationRecord::getStaticChain() const
{
  return staticChain;
}




ActivationRecord::~ActivationRecord()
{
  // dtor
  
  delete [] entries;
}



LambdaObject *&ActivationRecord::operator[](int i)
{
  /*
  // ### This bounds checking probably slows it down alot...
  if(i<0 || i>=numEntries)
    {
      ostringstream os;
      os << "Invalid index (" << i << ") in ActivationRecord::[]";
      throw LambdaException(os.str());
    }
  */

  return entries[i];
}



LambdaObject *ActivationRecord::getEntry(int i) const
{
  // use bounds checking
  //return (*const_cast<ActivationRecord*>(this))[i];

  return entries[i];
}



void ActivationRecord::setEntry(int i,LambdaObject *to)
{
  // use bounds checking
  (*this)[i]=to;

  //entries[i]=to;
}



int ActivationRecord::getNumEntries() const
{
  return numEntries;
}



void ActivationRecord::pushAccessibles(MarkStack &markStack)
{
  markStack.push(staticChain);
  int i; 
  for(i=0 ; i<numEntries ; ++i)
    markStack.push(entries[i]);
}




// ****************************************
//	   ExpandableAR methods
// ****************************************

ExpandableAR::ExpandableAR(ActivationRecord *staticChain)
  : ActivationRecord(0,staticChain), elasticArray(NULL)
{
  // ctor
}



ExpandableAR::~ExpandableAR()
{
  // dtor
}



LambdaObject *&ExpandableAR::operator[](int i)
{
  //  ### uncomment this if you have strange errors
  //if(i<0) throw LambdaException("Bad index in ExpandableAR::operator[]");

  if(i>=numEntries) numEntries=i+1;
  return (LambdaObject*&)(elasticArray[i]);
}



LambdaObject *ExpandableAR::getEntry(int i) const
{
  ExpandableAR &self=*const_cast<ExpandableAR*>(this);
  return self[i];
}



void ExpandableAR::setEntry(int i,LambdaObject *to)
{
  (*this)[i]=to;
}



void ExpandableAR::pushAccessibles(MarkStack &markStack)
{
  markStack.push(getStaticChain());
  for(int i=0 ; i<numEntries ; ++i)
    markStack.push(static_cast<Garbage*>(elasticArray[i]));
}


