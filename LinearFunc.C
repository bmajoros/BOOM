/***********************************************************************
 LinearFunc.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#include "LinearFunc.H"
using namespace std;
using namespace BOOM;


BOOM::LinearFunc::LinearFunc(double slope,
			       double intercept,
			       double coefOfDeterm)
  : slope(slope), intercept(intercept),
    coefOfDeterm(coefOfDeterm)
{  
}



BOOM::LinearFunc::LinearFunc()
  : slope(1.0), intercept(0.0), coefOfDeterm(0.0)
{  
}



double BOOM::LinearFunc::operator()(double x) const
{
  return slope*x+intercept;
}



BOOM::LinearFunc &BOOM::LinearFunc::operator=(const BOOM::LinearFunc &f)
{
  slope=f.slope;
  intercept=f.intercept;
  coefOfDeterm=f.coefOfDeterm;
}



double BOOM::LinearFunc::getCoefDeterm() const
{
  return coefOfDeterm;
}



void BOOM::LinearFunc::printOn(ostream &os) const
{
  os<<slope<<" "<<intercept<<" "<<coefOfDeterm;
}



void BOOM::LinearFunc::readFrom(istream &is)
{
  is>>slope>>intercept>>coefOfDeterm;
}



ostream &BOOM::operator<<(ostream &os,const LinearFunc &f)
{
  f.printOn(os);
  return os;
}



istream &BOOM::operator>>(istream &is,LinearFunc &f)
{
  f.readFrom(is);
  return is;
}


