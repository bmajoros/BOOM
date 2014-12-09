/****************************************************************
 MultivariateDistribution.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <fstream>
#include "MultivariateDistribution.H"
#include "BOOM/Exceptions.H"
#include "MultiGauss.H"
#include "Random.H"
using namespace std;
using namespace GSL;


/****************************************************************
                   MultivariateType methods
 ****************************************************************/

ostream &operator<<(ostream &os,MultivariateType t)
{
  switch(t) 
    {
    case MVD_NONE: os<<"NONE"; break;
    case MVD_GAUSS:os<<"GAUSS"; break;
    default: INTERNAL_ERROR;
    }
  return os;
}




/****************************************************************
               MultivariateDistribution methods
 ****************************************************************/

MultivariateDistribution::MultivariateDistribution()
  : dim(0), type(MVD_NONE), generator(Random::getGenerator())
{
  // default ctor
}



MultivariateDistribution::MultivariateDistribution(int dim,
						   MultivariateType type)
  : dim(dim), type(type), generator(Random::getGenerator())
{
  // ctor
}



int MultivariateDistribution::getDim() const
{
  return dim;
}



MultivariateType MultivariateDistribution::getType() const
{
  return type;
}



MultivariateDistribution *MultivariateDistribution::load(const 
							 String &filename)
{
  ifstream is(filename.c_str());
  int typeNum, dim;
  is>>typeNum;
  MultivariateType type=static_cast<MultivariateType>(typeNum);
  is>>dim;
  MultivariateDistribution *distr;
  switch(type)
    {
    case MVD_GAUSS:
      distr=new MultiGauss;
      distr->dim=dim;
      distr->load(is);
      break;
    case MVD_NONE:
    default: 
      throw String("Bad MultivariateType code: ")+typeNum;
    }
  return distr;
}



void MultivariateDistribution::save(const String &filename) const
{
  ofstream os(filename.c_str());
  os<<static_cast<int>(type)<<endl;
  os<<dim<<endl;
  save(os);
}



ostream &GSL::operator<<(ostream &os,const MultivariateDistribution &d)
{
  d.printOn(os);
  return os;
}





