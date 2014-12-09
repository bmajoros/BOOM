/*
 ObjectPrinter.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "ObjectPrinter.H"
#include "Lists.H"
#include "AstPrinter.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


ObjectPrinter::ObjectPrinter(RunTimeEnvironment &env)
  : env(env)
{
  // ctor
}



void ObjectPrinter::print(LambdaObject *object,ostream &os)
{
  if(object==NULL)
    {
      os << "nil";
      return;
    }

  switch(object->getType())
    {
    case OBTYPE_INT:
      printInt(static_cast<LambdaInt*>(object),os);
      break;
    case OBTYPE_CHAR:
      printChar(static_cast<LambdaChar*>(object),os);
      break;
    case OBTYPE_FLOAT:
      printFloat(static_cast<LambdaFloat*>(object),os);
      break;
    case OBTYPE_STRING:
      printString(static_cast<LambdaString*>(object),os);
      break;
    case OBTYPE_CLOSURE:
      printClosure(static_cast<Closure*>(object),os);
      break;
    case OBTYPE_FOREIGN_FUNCTION:
      printForeignFunc(
        static_cast<ForeignFunctionObject*>(object),os);
      break;
    case OBTYPE_CONS_CELL:
      printList(static_cast<ConsCell*>(object),os);
      break;
    case OBTYPE_FOREIGN_TYPE:
      printForeignType(object,os);
      break;
    case OBTYPE_SYMBOL:
      printSymbol(static_cast<Symbol*>(object),os);
      break;
    case OBTYPE_VECTOR:
      printVector(static_cast<LambdaVector*>(object),os);
      break;
    }
}



void ObjectPrinter::printVector(LambdaVector *v,ostream &os)
{
  os << "[";

  const int num=v->getSize();
  int i;
  for(i=0 ; i<num ; ++i)
    {
      print(v->getElement(i),os);
      if(i<num-1)
	os << ' ';
    }

  os << "]";
}



void ObjectPrinter::printInt(LambdaInt *lambdaInt,ostream &os)
{
  os << lambdaInt->getValue();
}



void ObjectPrinter::printChar(LambdaChar *lambdaChar,ostream &os)
{
  os << '#' << lambdaChar->getValue();
}



void ObjectPrinter::printFloat(LambdaFloat *lambdaFloat,ostream &os)
{
  os << lambdaFloat->getValue();
}



void ObjectPrinter::printString(LambdaString *lambdaString,ostream &os)
{
  os << '\"' << lambdaString->getValue() << '\"';
}



void ObjectPrinter::printClosure(Closure *closure,ostream &os)
{
  AstPrinter printer(os,env);
  printer.visit(*closure->getFunction());
}



void ObjectPrinter::printForeignFunc(ForeignFunctionObject *function,
				     ostream &os)
{
  os << "[external function]";
}



void ObjectPrinter::printList(ConsCell *consCell,ostream &os)
{
  if(isList(consCell))
    printProperList(consCell,os);
  else
    printImproperList(consCell,os);
}



void ObjectPrinter::printProperList(ConsCell *consCell,ostream &os)
{
  os << "\'(";
  while(consCell)
    {
      print(consCell->getCar(),os);
      consCell=static_cast<ConsCell*>(consCell->getCdr());
      if(consCell!=NULL)
	os << ' ';
    }
  os << ')';
}



void ObjectPrinter::printImproperList(ConsCell *consCell,ostream &os)
{
  os << '(';
  while(consCell)
    {
      print(consCell->getCar(),os);
      LambdaObject *cdr=consCell->getCdr();
      if(cdr->getType()!=OBTYPE_CONS_CELL)
	{
	  print(cdr,os);
	  break;
	}
      consCell=static_cast<ConsCell*>(cdr);
      os << " . ";
    }
  os << ')';
}



void ObjectPrinter::printForeignType(LambdaObject *foreignType,ostream &os)
{
  os << "<external type>";
}



void ObjectPrinter::printSymbol(Symbol *symbol,ostream &os)
{
  os << '\'' << symbol->getLexeme();
}

