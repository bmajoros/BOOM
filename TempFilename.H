/****************************************************************
 TempFilename.H
 Copyright (C)2015 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_TempFilename_H
#define INCL_TempFilename_H
#include <iostream>
#include "String.H"
using namespace std;
namespace BOOM {

class TempFilename {
public:
  static String get();
};

}
#endif

