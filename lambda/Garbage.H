/*
 Garbage.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_Garbage_H
#define INCL_Garbage_H

#include "MarkStack.H"
#include "MarkHook.H"

using namespace std;
using namespace BOOM;

// Base class for garbage-collectable objects.  The garbage-
// collector only handles objects derived from this class.

namespace Lambda {
class Garbage : public MarkHook
{
  bool marked;
public:
  Garbage();
  void mark();
  void unMark();
  bool isMarked() const;
};
}

#endif
