/***********************************************************************
 ContinuousDistr.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_ContinuousDistr_H
#define INCL_ContinuousDistr_H

namespace BOOM {
class ContinuousDistr
{
public:
  virtual double density(double x) const=0;
  virtual double logDensity(double x) const=0;
};
}

#endif
