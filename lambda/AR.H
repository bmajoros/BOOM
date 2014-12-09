/*
 AR.H (ActivationRecords)
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_AR_H
#define INCL_AR_H

#include "LambdaObject.H"
#include "BOOM/ElasVector.H"
using namespace std;
using namespace BOOM;

/*
 An ActivationRecord is a record on the RunTimeStack
 representing a single invocation of a function.  It
 contains actual parameters for the function.  
 The "next" pointer (inherited from link_node indirectly 
 through Garbage) is referred to in compiler terminology 
 as the "dynamic chain," because it represents a dynamic 
 relationship between functions; i.e., the call trace.
 The "static chain" is stored explicitly in this class;
 it is used to resolve variables according to lexical
 depth and position (as in Debruin Notation).  The
 lexical depth is the # static chains to follow, and
 lexical position is the position in that AR of the
 slot holding the desired value.
*/

namespace Lambda {
class ActivationRecord : public Garbage 
{
  LambdaObject **entries;
  ActivationRecord *staticChain;
protected:
  int numEntries;
public:
  ActivationRecord(int numEntries,
		   ActivationRecord *staticChain=NULL);
  ~ActivationRecord();
  virtual LambdaObject *&operator[](int i);
  virtual LambdaObject *getEntry(int i) const;
  virtual void setEntry(int i,LambdaObject *to);
  virtual int getNumEntries() const;
  virtual ActivationRecord *getStaticChain() const;
  virtual void pushAccessibles(MarkStack &);
};



class ExpandableAR : public ActivationRecord
{
  ElasVector<LambdaObject*> elasticArray;
public:
  ExpandableAR(ActivationRecord *staticChain=NULL);
  ~ExpandableAR();
  LambdaObject *&operator[](int i);
  LambdaObject *getEntry(int i) const;
  void setEntry(int i,LambdaObject *to);
  virtual void pushAccessibles(MarkStack &);
};

}

#endif
