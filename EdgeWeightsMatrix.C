/****************************************************************
 EdgeWeightsMatrix.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "EdgeWeightsMatrix.H"
using namespace std;
using namespace BOOM;


EdgeWeightsMatrix::EdgeWeightsMatrix(int numVertices)
  : M(numVertices,numVertices)
{
  // ctor
}



double EdgeWeightsMatrix::getWeight(int vertex1,int vertex2) const
{
  return M[vertex1][vertex2];
}



void EdgeWeightsMatrix::setWeight(int vertex1,int vertex2,double weight)
{
  M[vertex1][vertex2]=weight;
}



