/****************************************************************
 EdgeWeights.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_EdgeWeights_H
#define INCL_EdgeWeights_H
#include <iostream>
using namespace std;


namespace BOOM {

class EdgeWeights
{
public:
  virtual double getWeight(int vertex1,int vertex2) const=0;
};

}

#endif

