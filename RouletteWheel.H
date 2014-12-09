/****************************************************************
 RouletteWheel.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_RouletteWheel_H
#define INCL_RouletteWheel_H
#include <iostream>
#include "Vector.H"
using namespace std;

namespace BOOM {


class RouletteWheel
{
  Vector<double> wheel; // cumulative scores
  int lookup(double);
public:
  void addSector(double probability);
  void doneAddingSectors(); // DOES *NOT* NORMALIZE PROBABILITIES!
  int spin();
  void printOn(ostream &) const;
};

ostream &operator<<(ostream &,const RouletteWheel &);

}

#endif
