/*
 Lists.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Lists.H"
#include "LambdaException.H"
using namespace std;
using namespace BOOM;
namespace Lambda {


bool isList(ConsCell *head)
{
  LambdaObject *object=head;

  while(object)
    {
      if(object->getType()==OBTYPE_CONS_CELL)
	object=static_cast<ConsCell*>(object)->getCdr();
      else
	return false;
    }
  return true;
}



int listLength(ConsCell *list)
{
  int len=0;
  while(list)
    {
      ++len;
      LambdaObject *cdr=list->getCdr();
      if(!cdr) break;
      if(cdr->getType()!=OBTYPE_CONS_CELL)
	throw LambdaException("Invalid list");
      list=static_cast<ConsCell*>(cdr);
    }
  return len;
}



ConsCell *VectorToList(LambdaVector *vector)
{
  const int len=vector->getSize();
  ConsCell *list=NULL;

  int i;
  for(i=len-1 ; i>=0 ; --i)
    list=new ConsCell(vector->getElement(i),list);

  return list;
}



LambdaVector *ListToVector(ConsCell *list)
{
  const int len=listLength(list);
  LambdaVector *vector=new LambdaVector(len);

  int i;
  ConsCell *cell=list;
  for(i=0 ; i<len ; ++i)
    {
      vector->setElement(i,cell->getCar());
      cell=static_cast<ConsCell*>(cell->getCdr());
    }

  return vector;
}

}


