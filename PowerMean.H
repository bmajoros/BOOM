/****************************************************************
 PowerMean.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_PowerMean_H
#define INCL_PowerMean_H
#include <iostream>
#include "Vector.H"
using namespace std;
namespace BOOM {

class PowerMean {
public:
  static float compute_log(Vector<float> &,float power);
};

}
#endif

