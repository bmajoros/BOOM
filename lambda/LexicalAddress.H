/*
 LexicalAddress.H
 BOOM : Bioinformatics Object Oriented Modules
 Copyright (C)2012 William H. Majoros (martiandna@gmail.com).
 This is OPEN SOURCE SOFTWARE governed by the Gnu General Public
 License (GPL) version 3, as described at www.opensource.org.
*/
#ifndef INCL_LexicalAddress_H
#define INCL_LexicalAddress_H
#include <iostream>
using namespace std;

namespace Lambda {
class LexicalAddress
{
  int depth, position;
public:
  LexicalAddress(int depth,int position);
  int getDepth() const;
  int getPosition() const;
  bool operator==(const LexicalAddress &la) const
    { return depth==la.depth && position==la.position; }
  void changeDepth(int d) { depth=d; }
  void changePosition(int p) { position=p; }
  void printOn(ostream &os) const {os<<"(: "<<depth<<" "<<position<<" )"<<endl;}
};

inline ostream &operator<<(ostream &os,const LexicalAddress &la) {
  la.printOn(os);
  return os;
}

}

#endif
