/****************************************************************
 IlluminaQual.C
 Copyright (C)2021 William H. Majoros (bmajoros@alumni.duke.edu)
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include <math.h>
#include <string.h>
#include "IlluminaQual.H"
using namespace std;
using namespace BOOM;

const char IlluminaQual::CHAR_CODES[]=
  "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJ";


IlluminaQual::IlluminaQual()
{
  for(int i=0 ; i<256 ; ++i) charMap[i]=-1;
  const int L=sizeof(CHAR_CODES);
  for(int i=0 ; i<L ; ++i) {
    const char c=CHAR_CODES[i];
    charMap[int(c)]=i;
  }
}



char IlluminaQual::phredToChar(int phred)
{
  if(phred>=sizeof(CHAR_CODES)) 
    throw "Invalid phred score in IlluminaQual::intToChar()";
  return CHAR_CODES[phred];
}



int IlluminaQual::charToPhred(char c)
{
  return charMap[c];
}



float IlluminaQual::charToErrorProb(char c)
{
  const int q=charMap[c];
  return phredToErrorProb(q);
}



float IlluminaQual::phredToErrorProb(int q)
{
  return pow(10.0,-float(q)/10.0);
}



