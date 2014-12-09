/*
 MarkStack.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "MarkStack.H"
#include "Garbage.H"
#include <iostream>
using namespace std;
using namespace BOOM;
using namespace Lambda;


MarkStack::MarkStack()
{
  // ctor
}



void MarkStack::push(Garbage *g)
{
  if(g)
    stack.push(g);
}



Garbage *MarkStack::pop()
{
  return stack.isEmpty() ? 
    NULL : 
    static_cast<Garbage *>(stack.pop());
}



void MarkStack::clear()
{
  stack.clear();
}






