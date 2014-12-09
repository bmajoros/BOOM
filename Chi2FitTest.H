/***********************************************************************
 Chi2FitTest.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ***********************************************************************/
#ifndef INCL_BOOM_Chi2FitTest_H
#define INCL_BOOM_Chi2FitTest_H
using namespace std;
#include "Vector.H"
#include "Chi2Table.H"

namespace BOOM {
class Chi2FitTest
{
public:
  Chi2FitTest(BOOM::Vector<int> &observedCounts,
		    BOOM::Vector<int> &expectedCounts,
		    BOOM::Chi2Table &);
  bool goodFit();
  double getChiSquared();
  double getP();
private:
  double P, chiSquared;
  bool fit;
  void performTest(BOOM::Vector<int> &observedCounts,
		   BOOM::Vector<int> &expectedCounts,
		   BOOM::Chi2Table &);
};
}

#endif
