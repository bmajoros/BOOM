/****************************************************************
 IlluminaQual.H
 Copyright (C)2021 William H. Majoros (bmajoros@alumni.duke.edu)
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_IlluminaQual_H
#define INCL_IlluminaQual_H
#include <iostream>
using namespace std;

namespace BOOM {
class IlluminaQual {
  static const char CHAR_CODES[];
  int charMap[256];
public:
  IlluminaQual();
  int charToPhred(char); // maps an encoded char from SAM file
  char phredToChar(int phred); // encodes a phred score as illumina char
  float charToErrorProb(char); // maps an encoded char from SAM file
  float phredToErrorProb(int phred); // maps integer quality score, not char
};
}

#endif

