/***********************************************************************
 MutInfo.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_MutInfo_H
#define INCL_BOOM_MutInfo_H
#include "JointDistr.H"
#include "ContingencyTbl.H"
using namespace std;

namespace BOOM {
class MutInfo
{
public:
  static double compute(BOOM::ContingencyTbl &);
  static double compute(const BOOM::JointDistr &);
};
}

#endif
