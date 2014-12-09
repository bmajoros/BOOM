/***********************************************************************
 Chi2IndepTest.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Chi2IndepTest_H
#define INCL_BOOM_Chi2IndepTest_H
using namespace std;
#include "Array2D.H"
#include "Chi2Table.H"

namespace BOOM {
class Chi2IndepTest
{
public:
  Chi2IndepTest(BOOM::Array2D<int> &contingencyTable,
		BOOM::Chi2Table &);
  bool areIndependent();
  double getChiSquared();
  double getP();
  inline bool areDependent() {return !areIndependent();}
private:
  double P, chiSquared;
  bool independent;
  void runTest(BOOM::Array2D<int> &,BOOM::Chi2Table &);
};
}

#endif
