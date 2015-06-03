/****************************************************************
 CodonIterator.C
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "CodonIterator.H"
using namespace std;
using namespace BOOM;

/****************************************************************
                          struct Codon
 ****************************************************************/

bool Codon::valid() const
{
  return codon.length()==3;
}



bool Codon::isStop() const
{
  return codon=="TAG" || codon=="TGA" || codon=="TAA";
}



/****************************************************************
                          class CodonIterator
 ****************************************************************/

CodonIterator::CodonIterator(const GffTranscript &transcript,
			     const String &substrate)
  : transcript(transcript), substrate(substrate)
{
  reset();
}


void CodonIterator::reset()
{
  if(transcript.numExons()<1) throw "Transcript has no exons in CodonIterator";
  currentExon=&transcript.getIthExon(0);
  posWithinExon=0;
}



bool CodonIterator::done() const
{
  return currentExon!=NULL && posWithinExon<=currentExon->length()-3;
}



Codon CodonIterator::nextCodon()
{

}




