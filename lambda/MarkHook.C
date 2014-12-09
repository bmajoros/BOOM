/*
 MarkHook.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "MarkHook.H"
#include "Garbage.H"
using namespace Lambda;

void Immortals::add(Garbage *g)
{
  immortals.push_back(g);
}



void Immortals::pushAccessibles(MarkStack &s)
{
  Vector<Garbage*>::iterator cur=immortals.begin(), end=immortals.end();
  for(; cur!=end ; ++cur)
    s.push(*cur);
}








