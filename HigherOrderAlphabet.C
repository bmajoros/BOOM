/****************************************************************
 HigherOrderAlphabet.C
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "HigherOrderAlphabet.H"
#include "NthOrderStringIterator.H"
#include "Math.H"
#include "Exceptions.H"
using namespace std;
using namespace BOOM;



/****************************************************************
                  HigherOrderAlphabet methods
 ****************************************************************/

HigherOrderAlphabet::HigherOrderAlphabet()
  : N(-1)
{
  // default ctor
}



HigherOrderAlphabet::HigherOrderAlphabet(const Alphabet &a,int N)
  : alphabet(a), N(N)
{
  init();
}



int HigherOrderAlphabet::getN() const
{
  return N;
}



const Alphabet &HigherOrderAlphabet::getBaseAlphabet() const
{
  return alphabet;
}



NmerSymbol HigherOrderAlphabet::lookup(const Sequence &S,int offset,
				   int L) const
{
  if(L<0) L=min(N,S.getLength());
  int weight=1, value=0, radix=alphabet.size();
  for(int i=0 ; i<L ; ++i) {
    NmerSymbol s=S[offset+L-i-1];
    value+=weight*s;
    weight*=radix;
  }
  return value+lambda[L];
}



int HigherOrderAlphabet::effectiveLength(NmerSymbol nmerSymbol) const
{
  int x=nmerSymbol;
  int radix=alphabet.size();
  int L=1;
  for(; L<=N ; ++L) if(lambda[L]>x) break;
  return L;
}


Sequence HigherOrderAlphabet::lookup(NmerSymbol nmerSymbol) const
{
  int x=nmerSymbol;
  int radix=alphabet.size();
  int L=1;
  for(; L<=N ; ++L) if(lambda[L]>x) break;
  --L;
  x-=lambda[L];
  Sequence S(Symbol(0),L);
  for(int i=0 ; i<L ; ++i) {
    int m=x%radix;
    S[L-i-1]=Symbol(m);
    x/=radix;
  }
  return S;
}



int HigherOrderAlphabet::getNumNmers() const
{
  return totalNumNmers;
}



void HigherOrderAlphabet::init()
{
  radix=alphabet.size();
  nmersPerLength.resize(N+1);
  lambda.resize(N+1);
  for(int o=0 ; o<=N ; ++o) nmersPerLength[o]=ipow(radix,o);
  lambda[0]=0;
  for(int L=1 ; L<=N ; ++L) lambda[L]=lambda[L-1]+nmersPerLength[L-1];
  totalNumNmers=lambda[N]+nmersPerLength[N];
}



BaseSymbol HigherOrderAlphabet::lastBase(NmerSymbol x) const
{
  int radix=alphabet.size();
  int L=1;
  for(; L<=N ; ++L) if(lambda[L]>x) break;
  --L;
  x-=lambda[L];
  int m=x%radix;
  return BaseSymbol(m);
}




/****************************************************************
                        NmerChain methods
 ****************************************************************/

NmerChain::NmerChain(const HigherOrderAlphabet &HOA)
  : N(HOA.getN())
{
  const Alphabet &alphabet=HOA.getBaseAlphabet();
  int radix=alphabet.size();
  int numNmers=HOA.getNumNmers();
  table.resize(numNmers,radix);
  prefixes.resize(numNmers); suffixes.resize(numNmers);
  prefixes[0]=suffixes[0]=0;
  for(int n=0 ; n<=N ; ++n) {
    NthOrderStringIterator iter(n,alphabet);
    while(!iter.done()) {
      Sequence from=iter.getNextSequence(), history, prefix, suffix;
      const int fromCode=HOA.lookup(from);
      Array2D<NmerSymbol>::RowIn2DArray<NmerSymbol> row=table[fromCode];
      if(n<N) history=from;
      else from.getSubsequence(1,N-1,history);
      for(Symbol s=0 ; s<radix ; ++s) {
	Sequence to=history+s;
	const int toCode=HOA.lookup(to);
	row[s]=toCode;
      }
      if(n>0) {
	from.getSubsequence(0,n-1,prefix);
	from.getSubsequence(1,n-1,suffix);
	prefixes[fromCode]=HOA.lookup(prefix);
	suffixes[fromCode]=HOA.lookup(suffix);
      }
    }
  }
}



NmerSymbol NmerChain::nextNmer(NmerSymbol currentNmer,
			       BaseSymbol nextBase) const
{
  return table[currentNmer][nextBase];
}



int NmerChain::getN() const
{
  return N;
}



NmerSymbol NmerChain::getPrefix(NmerSymbol s) const
{
  return prefixes[s];
}



NmerSymbol NmerChain::getSuffix(NmerSymbol s) const
{
  return suffixes[s];
}



bool NmerChain::isPrefix(NmerSymbol prefix,NmerSymbol fullNmer) const
{
  do {
    if(fullNmer==prefix) break;
    fullNmer=getPrefix(fullNmer);
  } while(fullNmer!=0);
  return fullNmer==prefix;
}



bool NmerChain::isSuffix(NmerSymbol fullNmer,NmerSymbol suffix) const
{
  do {
    if(fullNmer==suffix) break;
    fullNmer=getSuffix(fullNmer);
  } while(fullNmer!=0);
  return fullNmer==suffix;
}



