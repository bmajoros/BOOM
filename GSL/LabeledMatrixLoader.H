/****************************************************************
 LabeledMatrixLoader.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_LabeledMatrixLoader_H
#define INCL_LabeledMatrixLoader_H
#include <iostream>
#include "Matrix.H"
#include "../AlphabetMap.H"
#include "../String.H"
#include "../Regex.H"
using namespace std;
using namespace BOOM;


namespace GSL {

class LabeledMatrixLoader
{
public:
  static GSL::Matrix *load(const Alphabet &,const AlphabetMap &,
			   const String &filename,double defaultValue=0);
  static void load(const Alphabet &,const AlphabetMap &,const String &filename,
		   GSL::Matrix &,double defaultValue=0);
private:
  const AlphabetMap &alphabetMap;
  static Regex commentRegex, headerRegex, tableRegex, errorRegex;
};

}

#endif
