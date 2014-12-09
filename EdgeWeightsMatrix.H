/****************************************************************
 EdgeWeightsMatrix.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_EdgeWeightsMatrix_H
#define INCL_EdgeWeightsMatrix_H
#include <iostream>
#include "EdgeWeights.H"
#include "Array2D.H"
using namespace std;


namespace BOOM {

class EdgeWeightsMatrix : public EdgeWeights
{
public:
  EdgeWeightsMatrix(int numVertices);
  virtual double getWeight(int vertex1,int vertex2) const;
  void setWeight(int vertex1,int vertex2,double weight);
private:
  Array2D<double> M;
};

}

#endif

