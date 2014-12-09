/*
 LambdaObject.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LambdaObject.H"
#include "BOOM/String.H"
using namespace Lambda;


ostream &Lambda::operator<<(ostream &os,LambdaObjectType t)
{
  switch(t) 
    {
    case OBTYPE_INT: os<<"int"; break;
    case OBTYPE_CHAR: os<<"char"; break;
    case OBTYPE_FLOAT: os<<"float"; break;
    case OBTYPE_STRING: os<<"string"; break;
    case OBTYPE_CLOSURE: os<<"closure"; break;
    case OBTYPE_FOREIGN_FUNCTION: os<<"foreign-function"; break;
    case OBTYPE_CONS_CELL: os<<"cons-cell"; break;
    case OBTYPE_SYMBOL: os<<"symbol"; break;
    case OBTYPE_FOREIGN_TYPE: os<<"foreign-object"; break;
    case OBTYPE_VECTOR: os<<"vector"; break;
    default: throw BOOM::String("Uknown LambdaObjectType: ")+int(t);
    }
  return os;
}



LambdaObject::LambdaObject(LambdaObjectType objectType)
  : objectType(objectType)
{
  // ctor
}



LambdaObjectType LambdaObject::getType()
{
  return objectType;
}




ForeignObject::ForeignObject(void *o,int tag) 
  : object(o), tag(tag), LambdaObject(OBTYPE_FOREIGN_TYPE) 
{
  // ctor
}



int ForeignObject::getTag() const 
{
  return tag;
}



void *ForeignObject::getObject() const 
{
  return object;
}



void ForeignObject::setObject(void *o)
{
  object=o;
}

