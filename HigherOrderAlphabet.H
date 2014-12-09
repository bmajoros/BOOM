/****************************************************************
 HigherOrderAlphabet.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_HigherOrderAlphabet_H
#define INCL_HigherOrderAlphabet_H
#include <iostream>
#include "Alphabet.H"
#include "Sequence.H"
#include "Array2D.H"
using namespace std;
using namespace BOOM;

namespace BOOM {

/****************************************************************
                   Some basic definitions
 ****************************************************************/
typedef int NmerSymbol; // higher-order
typedef Symbol BaseSymbol; // zeroth order
class HigherOrderAlphabet; // forward decl



/****************************************************************
                      class NmerChain

 This maps a higher-order symbol and a lower-order symbol to
 the next higher-order sybol; e.g., chain[ACCG][T]=CCGT
 ****************************************************************/
struct NmerChain {
  NmerChain() {}
  NmerChain(const HigherOrderAlphabet &);
  NmerSymbol nextNmer(NmerSymbol currentNmer,BaseSymbol nextBase) const;
  NmerSymbol getPrefix(NmerSymbol) const;
  NmerSymbol getSuffix(NmerSymbol) const;
  bool isPrefix(NmerSymbol prefix,NmerSymbol fullNmer) const;
  bool isSuffix(NmerSymbol fullNmer,NmerSymbol suffix) const;
  int getN() const;
protected:
  Array2D<NmerSymbol> table;
  Array1D<NmerSymbol> prefixes, suffixes;
  int N;
};



/****************************************************************
                      class HigherOrderAlphabet
 ****************************************************************/
class HigherOrderAlphabet {
public:
  HigherOrderAlphabet();
  HigherOrderAlphabet(const Alphabet &,int N); // N=order+1
  int getN() const;
  int getNumNmers() const; // includes all lower orders too
  inline int size() const {return getNumNmers();}
  const Alphabet &getBaseAlphabet() const;
  NmerSymbol lookup(const Sequence &,int offset=0,int L=-1) const;
  BaseSymbol lastBase(NmerSymbol) const;
  Sequence lookup(NmerSymbol) const;
  NmerChain *constructChain() const {return new NmerChain(*this);}
  int effectiveLength(NmerSymbol nmerSymbol) const;
protected:
  Alphabet alphabet; // base alphabet (0th order)
  int N; // the "N" in "N-grams"
  int radix; // size of base alphabet
  int totalNumNmers; // includes lower orders too
  IntArray1D nmersPerLength; // includes only Nmers of *exactly* that length
  IntArray1D lambda; // lambda[L]=total # Nmers of length less than L
  void init();
};

}
#endif

