/*
 LambdaObject.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LambdaObject_H
#define INCL_LambdaObject_H

#include "Garbage.H"
using namespace std;
using namespace BOOM;

namespace Lambda {

// **************************************************
//                enum LambdaObjectType
// **************************************************

enum LambdaObjectType
{
  OBTYPE_INT,
  OBTYPE_CHAR,
  OBTYPE_FLOAT,
  OBTYPE_STRING,
  OBTYPE_CLOSURE,
  OBTYPE_FOREIGN_FUNCTION,
  OBTYPE_CONS_CELL,
  OBTYPE_SYMBOL,
  OBTYPE_FOREIGN_TYPE, // use this for your objects
  OBTYPE_VECTOR
};

ostream &operator<<(ostream &,LambdaObjectType);


// **************************************************
//                class LambdaObject 
// **************************************************

class LambdaObject : public Garbage
{
  LambdaObjectType objectType;
public:
  LambdaObject(LambdaObjectType);
  LambdaObjectType getType();
  virtual void pushAccessibles(MarkStack &) {}
};



// **************************************************
//              class ForeignObject 
// **************************************************

/* To make your own C++ objects available within the
   lambda environment, you can either subclass
   LambdaObject (above), or just use ForeignObject
   (below) which has a void* that you can point at
   your C++ object.  The former solution is safer and 
   more elegant, so use the following class only if 
   you're feeling lazy.
 */

class ForeignObject : public LambdaObject {
  int tag; // use this however you want---i.e., as an enum or whatever
  void *object;
public:
  ForeignObject(void *o=NULL,int tag=0);
  int getTag() const;
  void *getObject() const;
  void setObject(void *);
};

}

#endif
