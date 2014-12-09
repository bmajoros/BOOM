/*
 Lists.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_Lists_H
#define INCL_Lists_H

#include "ConsCell.H"
#include "LambdaVector.H"

namespace Lambda {
bool isList(ConsCell *);
int listLength(ConsCell *);
ConsCell *VectorToList(LambdaVector *);
LambdaVector *ListToVector(ConsCell *);
}

#endif
