/*
 Garbage.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "Garbage.H"
using namespace std;
using namespace BOOM;
using namespace Lambda;


Garbage::Garbage()
  : marked(false)
{
  // ctor
}



void Garbage::mark()
{
  marked=true;
}



void Garbage::unMark()
{
  marked=false;
}



bool Garbage::isMarked() const
{
  return marked;
}


