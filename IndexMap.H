/****************************************************************
 IndexMap.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
 ****************************************************************/
#ifndef INCL_IndexMap_H
#define INCL_IndexMap_H
#include <iostream>
#include "Array1D.H"
#include "List.H"
using namespace std;

/*
  Maps positions in one sequence to positions in another sequence, or to
  "no position" (i.e., an indel) = -1.
 */

namespace BOOM {
class IndexMap {
public:
  static const int UNDEFINED;
  IndexMap(int length=0);
  void save(const String &filename); // uses run-length encoding... 
  bool load(const String &filename); //                   ...very compact!
  void save(File &);
  bool load(File &);
  void clear();
  void resize(int,bool copyData=true);
  void invert(int length,IndexMap &into);
  void compose(const IndexMap &with,IndexMap &into); // maps y values
  int operator [](int) const;
  int &operator [](int);
  void map(int from,int to);
  void setAllTo(int);
  int size() const;
  void printOn(ostream &) const;
  List<int> *asList();
  void copyFromList(List<int> &);
  bool sanityCheck(int targetLen);// also checks for monotonicity
  int countNonzero();
private:
  static const unsigned char MATCH, INSERT, DELETE, END, MAX_RUNLEN;
  Array1D<int> array;
  void getChunk(int pos,int parentPos,unsigned char &op,
		unsigned char &len);
};

ostream &operator<<(ostream &,const IndexMap &);
}

#endif

