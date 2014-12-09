/*
 LexicalAddress.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#include "LexicalAddress.H"
using namespace Lambda;


LexicalAddress::LexicalAddress(int depth,int position)
  : depth(depth), position(position)
{
  // ctor
}



int LexicalAddress::getDepth() const
{
  return depth;
}



int LexicalAddress::getPosition() const
{
  return position;
}








