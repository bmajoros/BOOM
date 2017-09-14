/****************************************************************
 Bed3Record.C
 Copyright (C)2017 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#include <iostream>
#include "Bed3Record.H"
using namespace std;
using namespace BOOM;

Bed3Record::Bed3Record(const String &chr,int begin,int end)
  : chr(chr), interval(begin,end)
{
  // ctor
}


